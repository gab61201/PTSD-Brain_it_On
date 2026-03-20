#include "GameWorld/PhysicalWorld.hpp"
#include "GameWorld/BaseObject.hpp"
#include "Level/PassCondition.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

#define STROKE_WIDTH 10.0F

namespace GameWorld {

class DrawingRayCastCallback : public b2RayCastCallback {
   public:
    bool hit = false;
    b2Vec2 hitPoint;
    b2Body* ignoreBody = nullptr;

    float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& /*normal*/, float fraction) override {
        if (fixture->GetBody() == ignoreBody) {
            return -1.0f;  // 忽略這條線本身包含的所有 fixtures
        }
        hit = true;
        hitPoint = point;
        return fraction;
    }
};

PhysicalWorld::PhysicalWorld(
    std::vector<std::shared_ptr<CompositeObject>> compositeObjects,
    PassCondition* passCondition)
    : m_b2World(b2Vec2(0.0f, -9.8f)),
      m_CompositeObject(std::move(compositeObjects)),
      m_PassCondition(passCondition) {
    for (auto& obj : m_CompositeObject) {
        obj->AttachToWorld(&m_b2World);
    }
    m_b2World.SetContactListener(m_PassCondition);
}

void PhysicalWorld::Start() {
    m_IsActive = true;
}

void PhysicalWorld::Stop() {
    m_IsActive = false;
}

void PhysicalWorld::DrawObject(glm::vec2 position) {
    // 檢查位置是否合法
    // ......

    // 無正在畫的物件則先建立
    if (m_LastDrawingObject == nullptr) {
        m_LastDrawingObject = std::make_shared<DrawnObject>(position);
        m_DrawnObjects.push_back(m_LastDrawingObject);
        m_LastDrawingObject->AttachToWorld(&m_b2World);
    } else {
        m_LastDrawingObject->DrawNextPoint(position);
    }
}

void PhysicalWorld::EndDrawing() {
    m_LastDrawingObject->EndDrawing();
    m_LastDrawingObject = nullptr;
}

bool PhysicalWorld::IsPassed() {
    return m_PassCondition->Check();
}
// ==========================================
// 每一幀的更新 (Update) - 遊戲主迴圈會呼叫這裡
// ==========================================
void PhysicalWorld::Update() {
    m_b2World.Step(m_IsActive / 60.0F, 8, 3);
    for (auto& obj : m_CompositeObject) {
        obj->Update();
    }
    for (auto& obj : m_DrawnObjects) {
        obj->Update();
    }
}

}  // namespace GameWorld
