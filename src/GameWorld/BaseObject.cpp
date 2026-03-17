#include "GameWorld/BaseObject.hpp"

#include <cmath>

#include "GameWorld/CoordinateHelper.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

#define IMAGE_SIZE 417.0F

namespace GameWorld {

// ==========================================
// 建構子 (Constructor)
// ==========================================

BaseObject::BaseObject(
    ShapeType shape,
    glm::vec2 size,
    glm::vec2 position,
    float rotation,
    bool isSensor)
    : m_ShapeType(shape),  // 記得在 .hpp 補上這個成員
      m_Size(size),
      m_RelativePosition(position),
      m_RelativeRotation(rotation),
      m_IsSensor(isSensor),  // 記得在 .hpp 補上這個成員
      m_Fixture(nullptr),
      m_Visual(std::make_shared<Util::GameObject>()) {}

// ==========================================
// 實體組裝期 (AttachToBody) - 現場製造零件並掛載
// ==========================================
void BaseObject::AttachToBody(b2Body* body) {
    // 防呆：如果已經有實體了，不要重複掛載
    if (m_Fixture != nullptr) return;

    // 1. 準備共用的物理設計圖
    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = m_IsSensor;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;

    // 2. 根據儲存的形狀資料，現場宣告形狀並掛載
    if (m_ShapeType == ShapeType::CIRCLE) {
        m_Visual->SetDrawable(std::make_shared<Util::Image>("Resources/Images/circle.png"));

        b2CircleShape circleShape;

        // 像素轉公尺
        circleShape.m_radius = GameWorld::PixelsToMeters(m_Size.x / 2.0F);
        // 設定相對偏移位置 (Local Position)
        circleShape.m_p = GameWorld::PixelsToMeters(m_RelativePosition);

        fixtureDef.shape = &circleShape;

        // 🎯 趁著 circleShape 還活著，立刻掛載並接住實體指標
        m_Fixture = body->CreateFixture(&fixtureDef);

    } else if (m_ShapeType == ShapeType::RECTANGLE) {
        m_Visual->SetDrawable(std::make_shared<Util::Image>("Resources/Images/square.png"));

        b2PolygonShape boxShape;

        // 矩形需要半寬與半高
        float hx = GameWorld::PixelsToMeters(m_Size.x / 2.0F);
        float hy = GameWorld::PixelsToMeters(m_Size.y / 2.0F);

        // 中心點是相對位置
        b2Vec2 center = GameWorld::PixelsToMeters(m_RelativePosition);

        // 呼叫 Box2D 專屬方法：(半寬, 半高, 相對中心點, 相對旋轉角度)
        boxShape.SetAsBox(hx, hy, center, m_RelativeRotation);

        fixtureDef.shape = &boxShape;

        // 🎯 立刻掛載並接住實體指標
        m_Fixture = body->CreateFixture(&fixtureDef);

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
    m_Visual->Draw();
}

}  // namespace GameWorld