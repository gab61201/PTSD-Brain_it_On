#include "GameWorld/Shape/Capsule.hpp"

namespace {

float IMAGE_SIZE = 417.0F;
std::string CIRCLE_IMAGE_PATH = "Resources/Images/BasicShapes/white_circle.png";
std::string RECTANGLE_IMAGE_PATH = "Resources/Images/BasicShapes/white_square.png";

}  // namespace

namespace GameWorld {

Capsule::Capsule(
    float diameter,
    const glm::vec2& pointA,
    const glm::vec2& pointB,
    bool isSensor)
    : Shape(diameter, (pointA+pointB) * 0.5f, std::atan2(pointB.y - pointA.y, pointB.x - pointA.x), isSensor),
    m_PointA(pointA), m_PointB(pointB) {}

void Capsule::AttachToBody(b2BodyId body) {
    if (B2_IS_NON_NULL(m_b2ShapeId)) {
        return;
    }
    if (m_Visual == nullptr) {
        m_Visual = std::make_shared<Util::GameObject>();
    }

    float diameter = std::get<float>(m_Size);
    b2Capsule capsuleShape = {
        PixelsToMeters(m_PointA),  // 第一個半圓中心
        PixelsToMeters(m_PointB),  // 第二個半圓中心
        PixelsToMeters(diameter * 0.5f)    // 半徑
    };
    
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = m_IsSensor;
    m_b2ShapeId = b2CreateCapsuleShape(body, &shapeDef, &capsuleShape);

    // 處理長度小於等於寬度的情況（顯示為圓形）
    if (glm::distance(m_PointA, m_PointB) <= 1.0f) {
        // 顯示為圓形
        m_Visual->SetDrawable(s_ImageCache.Get(CIRCLE_IMAGE_PATH));
        m_Visual->m_Transform.scale = glm::vec2(diameter, diameter) / IMAGE_SIZE;
    } else {
        // 中間矩形
        m_Visual = std::make_shared<Util::GameObject>();
        m_Visual->SetDrawable(s_ImageCache.Get(RECTANGLE_IMAGE_PATH));
        m_Visual->m_Transform.scale = glm::vec2(glm::distance(m_PointA, m_PointB), diameter) / IMAGE_SIZE;
        // 左圓
        m_CircleAVisual = std::make_shared<Util::GameObject>();
        m_CircleAVisual->SetDrawable(s_ImageCache.Get(CIRCLE_IMAGE_PATH));
        m_CircleAVisual->m_Transform.scale = glm::vec2(diameter, diameter) / IMAGE_SIZE;
        m_Visual->AddChild(m_CircleAVisual);
        // 右圓
        m_CircleBVisual = std::make_shared<Util::GameObject>();
        m_CircleBVisual->SetDrawable(s_ImageCache.Get(CIRCLE_IMAGE_PATH));
        m_CircleBVisual->m_Transform.scale = glm::vec2(diameter, diameter) / IMAGE_SIZE;
        m_Visual->AddChild(m_CircleBVisual);
    }
}

void Capsule::Update(glm::vec2 ParentObjectPosition, float ParentObjectRotation) {
    if (!m_Visual) return;
    // 1. 計算絕對旋轉角度 = 父物件目前的旋轉 + 自己的相對旋轉
    float globalRotation = ParentObjectRotation + m_RelativeRotation;

    // 2. 計算「公轉」後的相對座標 (2D 旋轉矩陣)
    // 必須使用父物件的旋轉角度 (ParentObjectRotation) 來進行公轉
    // 注意：C++ 的 std::cos / std::sin 接收的是弧度 (Radians)
    float cosTheta = std::cos(ParentObjectRotation);
    float sinTheta = std::sin(ParentObjectRotation);

    glm::vec2 rotatedLocalPos;
    rotatedLocalPos.x = m_RelativePosition.x * cosTheta - m_RelativePosition.y * sinTheta;
    rotatedLocalPos.y = m_RelativePosition.x * sinTheta + m_RelativePosition.y * cosTheta;

    // 3. 計算出在螢幕上的最終絕對座標 (父物件的世界座標 + 旋轉後的相對偏移量)
    glm::vec2 globalPosition = ParentObjectPosition + rotatedLocalPos;

    // 4. 同步資料給你的 PTSD GameObject
    m_Visual->m_Transform.translation = globalPosition;
    m_Visual->m_Transform.rotation = globalRotation;

    float halfLength = glm::distance(m_PointA, m_PointB) * 0.5f;    // 2. 計算膠囊體在「世界空間」中的 X 軸方向向量
    float cosGlobal = std::cos(globalRotation);
    float sinGlobal = std::sin(globalRotation);
    glm::vec2 globalDirection(cosGlobal, sinGlobal);

    // 3. 順著方向推算左右圓心的絕對座標
    // 右圓：中心點 + (方向向量 * 距離)
    glm::vec2 CircleAGlobalPos = globalPosition - (globalDirection * halfLength);
    m_CircleAVisual->m_Transform.translation = CircleAGlobalPos;
    // 左圓：中心點 - (方向向量 * 距離)
    glm::vec2 CircleBGlobalPos = globalPosition + (globalDirection * halfLength);
    m_CircleBVisual->m_Transform.translation = CircleBGlobalPos;
}

}  // namespace GameWorld
