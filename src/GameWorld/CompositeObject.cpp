#include "GameWorld/CompositeObject.hpp"

#include "GameWorld/CoordinateHelper.hpp"

namespace GameWorld {

// 💡 假設你的像素與公尺轉換工具寫在這個標頭檔，請依照你的專案架構調整
// #include "GameWorld/GameWorld.hpp"

// ==========================================
// 建構子 (Constructor)
// 注意：.cpp 實作檔中不可加上預設值 (如 = BodyType::STATIC)
// ==========================================
CompositeObject::CompositeObject(
    std::vector<std::shared_ptr<Shape>> shapes,
    BodyType bodyType,
    glm::vec2 position,
    float rotation)
    : m_Shapes(std::move(shapes)),  // 使用 std::move 提升 vector 賦值效能
      m_BodyType(bodyType),
      m_Position(position),
      m_Rotation(rotation),
      m_b2BodyId(b2_nullBodyId)  // 初始化時，真實物理骨架尚未生成
{
    for (auto& shape : m_Shapes) {
        m_Renderer.AddChild(shape->GetVisual());
    }
}

// ==========================================
// 掛載到物理世界 (AttachToWorld)
// 由擁有 b2World 的類別 (例如 GameWorld 或 Level) 呼叫
// ==========================================
void CompositeObject::AttachToWorld(b2WorldId world) {
    // 防呆機制：如果已經掛載過，就不再重複創建，避免記憶體洩漏
    if (B2_IS_NON_NULL(m_b2BodyId)) {
        return;
    }

    // 1. 準備剛體設計圖 (BodyDef)
    b2BodyDef bodyDef = b2DefaultBodyDef();

    // 將自訂的 BodyType 列舉轉換為 Box2D 原生型別
    switch (m_BodyType) {
        case BodyType::DYNAMIC:
            bodyDef.type = b2_dynamicBody;
            break;
        case BodyType::KINEMATIC:
            bodyDef.type = b2_kinematicBody;
            break;
        case BodyType::STATIC:
        default:
            bodyDef.type = b2_staticBody;
            break;
    }

    // 設定初始位置與角度
    // 注意：Box2D 必須使用公尺，角度使用弧度 (Radians)
    bodyDef.position = GameWorld::PixelsToMeters(m_Position);
    bodyDef.rotation = b2MakeRot(m_Rotation);

    // 2. 在物理世界中創建真實的剛體 (骨架)
    m_b2BodyId = b2CreateBody(world, &bodyDef);

    // 3. 向下交辦：呼叫所有子零件，讓它們把碰撞形狀掛載到這個骨架上
    for (auto& shape : m_Shapes) {
        if (shape) {
            // 這裡呼叫我們上一篇寫好的 Shape::AttachToBody
            shape->AttachToBody(m_b2BodyId);
        }
    }
}

// ==========================================
// 每一幀的畫面更新邏輯 (Update)
// ==========================================
void CompositeObject::Update() {
    // 如果骨架還沒生成，就沒有物理數據可以更新
    if (B2_IS_NULL(m_b2BodyId)) {
        return;
    }

    // 1. 從物理引擎取得這幀計算完的最新位置與角度
    b2Vec2 physicsPos = b2Body_GetPosition(m_b2BodyId);
    float currentRotation = b2Rot_GetAngle(b2Body_GetRotation(m_b2BodyId));

    // 2. 將物理的公尺座標，轉換回遊戲畫面的像素座標
    glm::vec2 currentPixelPos = GameWorld::MetersToPixels(physicsPos);

    // 3. 廣播更新：把最新的絕對座標與角度告訴所有子零件
    // 讓 Shape 內部的 2D 旋轉矩陣去計算各自圖片該擺放的位置
    for (auto& shape : m_Shapes) {
        shape->Update(currentPixelPos, currentRotation);
    }
    m_Renderer.Update();
}

}  // namespace GameWorld
