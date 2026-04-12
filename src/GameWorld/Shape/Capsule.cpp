#include "GameWorld/Shape/Capsule.hpp"

namespace {

float IMAGE_SIZE = 417.0F;
std::string CIRCLE_IMAGE_PATH = "Resources/Images/BasicShapes/white_circle.png";
std::string RECTANGLE_IMAGE_PATH = "Resources/Images/BasicShapes/white_square.png";

}  // namespace

namespace GameWorld {

std::shared_ptr<Capsule> Capsule::CreateFromPoints(
    const glm::vec2& pointA, 
    const glm::vec2& pointB, 
    float width, 
    bool isSensor) {
    auto capsule = std::make_shared<Capsule>();
    
    // 計算中心點
    glm::vec2 center = (pointA + pointB) * 0.5f;
    
    // 計算長度（兩點距離）
    float length = glm::length(pointB - pointA);
    
    // 計算方向角度
    float angle = std::atan2(pointB.y - pointA.y, pointB.x - pointA.x);
    
    // 設定基本屬性
    capsule->m_RelativePosition = center;
    capsule->m_RelativeRotation = angle;
    capsule->m_Size = glm::vec2(length, width);
    capsule->m_IsSensor = isSensor;
    
    return capsule;
}

std::shared_ptr<Capsule> Capsule::CreateFromCenter(
    const glm::vec2& center, 
    const glm::vec2& size, 
    float angle,
    bool isSensor) {
    auto capsule = std::make_shared<Capsule>();
    
    capsule->m_RelativePosition = center;
    capsule->m_RelativeRotation = angle;
    capsule->m_Size = size;  // x = length, y = width
    capsule->m_IsSensor = isSensor;
    
    return capsule;
}

void Capsule::AttachToBody(b2BodyId body) {
    if (B2_IS_NON_NULL(m_b2ShapeId)) {
        return;
    }
    if (m_Visual == nullptr) {
        m_Visual = std::make_shared<Util::GameObject>();
    }

    glm::vec2 size = std::get<glm::vec2>(m_Size);
    float length = size.x;
    float width = size.y;
    float radius = width / 2.0f;
    
    // 處理長度小於等於寬度的情況（顯示為圓形）
    if (length <= width) {
        b2Circle circleShape = {
            PixelsToMeters(m_RelativePosition),
            PixelsToMeters(radius)
        };
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.isSensor = m_IsSensor;
        m_b2ShapeId = b2CreateCircleShape(body, &shapeDef, &circleShape);
        
        // 視覺：只顯示一個圓
        auto circle = std::make_shared<Util::GameObject>();
        circle->SetDrawable(s_ImageCache.Get(CIRCLE_IMAGE_PATH));
        circle->m_Transform.scale = glm::vec2(width) / IMAGE_SIZE;
        m_Visual->AddChild(circle);
        return;
    }
    
    // 計算兩個半圓的中心點（考慮旋轉）
    float halfLength = length / 2.0f;
    
    // 計算旋轉後的偏移向量
    float cosAngle = std::cos(m_RelativeRotation);
    float sinAngle = std::sin(m_RelativeRotation);
    float offsetX = halfLength * cosAngle;
    float offsetY = halfLength * sinAngle;
    
    // Box2D b2Capsule 使用兩個半圓中心點
    glm::vec2 center1 = m_RelativePosition - glm::vec2(offsetX, offsetY);
    glm::vec2 center2 = m_RelativePosition + glm::vec2(offsetX, offsetY);
    
    b2Capsule capsuleShape = {
        PixelsToMeters(center1),  // 第一個半圓中心
        PixelsToMeters(center2),  // 第二個半圓中心
        PixelsToMeters(radius)    // 半徑
    };
    
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = m_IsSensor;
    m_b2ShapeId = b2CreateCapsuleShape(body, &shapeDef, &capsuleShape);
    
    // 視覺表示：左圓 + 中間矩形 + 右圓
    float rectWidth = length - width;
    
    // 左圓
    auto leftCircle = std::make_shared<Util::GameObject>();
    leftCircle->SetDrawable(s_ImageCache.Get(CIRCLE_IMAGE_PATH));
    leftCircle->m_Transform.translation = glm::vec2(-halfLength, 0);
    leftCircle->m_Transform.scale = glm::vec2(width) / IMAGE_SIZE;
    m_Visual->AddChild(leftCircle);
    
    // 中間矩形
    auto middleRect = std::make_shared<Util::GameObject>();
    middleRect->SetDrawable(s_ImageCache.Get(RECTANGLE_IMAGE_PATH));
    middleRect->m_Transform.translation = glm::vec2(0, 0);
    middleRect->m_Transform.scale = glm::vec2(rectWidth, width) / IMAGE_SIZE;
    m_Visual->AddChild(middleRect);
    
    // 右圓
    auto rightCircle = std::make_shared<Util::GameObject>();
    rightCircle->SetDrawable(s_ImageCache.Get(CIRCLE_IMAGE_PATH));
    rightCircle->m_Transform.translation = glm::vec2(halfLength, 0);
    rightCircle->m_Transform.scale = glm::vec2(width) / IMAGE_SIZE;
    m_Visual->AddChild(rightCircle);
}

}  // namespace GameWorld
