#include "GameWorld/BaseObject.hpp"

#include <cmath>

#include "GameWorld/CoordinateHelper.hpp"
#include "Util/GameObject.hpp"

#define IMAGE_SIZE 417.0F

namespace GameWorld {

// ==========================================
// 圖片快取 (Image Cache)
// ==========================================

Util::AssetStore<std::shared_ptr<Util::Image>> BaseObject::s_ImageCache(
    [](const std::string& path) {
        return std::make_shared<Util::Image>(path);
    }
);

// ==========================================
// 建構子 (Constructor)
// ==========================================

BaseObject::BaseObject(
    ShapeType shape,
    glm::vec2 size,
    glm::vec2 position,
    float rotation,
    bool isSensor)
    : m_Visual(std::make_shared<Util::GameObject>()),
        m_Fixture(b2_nullShapeId),
      m_ShapeType(shape),
      m_Size(size),
      m_RelativePosition(position),
      m_RelativeRotation(rotation),
      m_IsSensor(isSensor) {}

// ==========================================
// 實體組裝期 (AttachToBody) - 現場製造零件並掛載
// ==========================================
void BaseObject::AttachToBody(Physics::BodyPtr body) {
    // 防呆：如果已經有實體了，不要重複掛載
    if (B2_IS_NON_NULL(m_Fixture)) return;

    // 1. 準備共用的物理設計圖
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = m_IsSensor;
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;
    shapeDef.material.restitution = 0.5f;
    shapeDef.enableSensorEvents = true;
    shapeDef.enableContactEvents = true;

    // 2. 根據儲存的形狀資料，現場宣告形狀並掛載
    if (m_ShapeType == ShapeType::CIRCLE) {
        if (!m_IsSensor) {
            auto cachedImage = s_ImageCache.Get("Resources/Images/BasicShapes/white_circle.png");
            m_Visual->SetDrawable(cachedImage);
        }

        b2Circle circleShape = {};

        // 像素轉公尺
        circleShape.radius = GameWorld::PixelsToMeters(m_Size.x / 2.0F);
        // 設定相對偏移位置 (Local Position)
        circleShape.center = GameWorld::PixelsToMeters(m_RelativePosition);

        // 🎯 趁著 circleShape 還活著，立刻掛載並接住實體指標
        m_Fixture = b2CreateCircleShape(body, &shapeDef, &circleShape);

    } else if (m_ShapeType == ShapeType::RECTANGLE) {
        auto cachedImage = s_ImageCache.Get("Resources/Images/BasicShapes/white_square.png");
        m_Visual->SetDrawable(cachedImage);

        // 矩形需要半寬與半高
        float hx = GameWorld::PixelsToMeters(m_Size.x / 2.0F);
        float hy = GameWorld::PixelsToMeters(m_Size.y / 2.0F);

        // 中心點是相對位置
        b2Vec2 center = GameWorld::PixelsToMeters(m_RelativePosition);

        // 呼叫 Box2D 專屬方法：(半寬, 半高, 相對中心點, 相對旋轉角度)
        b2Polygon boxShape = b2MakeOffsetBox(hx, hy, center, b2MakeRot(m_RelativeRotation));

        // 🎯 立刻掛載並接住實體指標
        m_Fixture = b2CreatePolygonShape(body, &shapeDef, &boxShape);

    } else if (m_ShapeType == ShapeType::TRIANGLE) {
        // TODO: 未來的三角形實作
    }
    m_Visual->m_Transform.scale = m_Size / IMAGE_SIZE;
}

// ==========================================
// 每幀畫面更新 (Update) - 物理座標同步到圖片
// ==========================================
void BaseObject::Update(glm::vec2 ParentObjectPosition, float ParentObjectRotation) {
    // 如果沒有圖片可以更新，就直接返回以節省效能
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
}

}  // namespace GameWorld