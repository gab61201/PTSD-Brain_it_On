#include "GameWorld/PhysicalWorld.hpp"
#include "GameWorld/BaseObject.hpp"
#include "GameWorld/CoordinateHelper.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

#define STROKE_WIDTH 10.0F

namespace GameWorld {

class PointQueryCallback : public b2QueryCallback {
   public:
    bool hit = false;
    b2Vec2 testPoint;

    bool ReportFixture(b2Fixture* fixture) override {
        // 通常我們不把 Sensor 當作阻擋畫線的物體
        if (fixture->IsSensor()) return true;
        if (fixture->TestPoint(testPoint)) {
            hit = true;
            return false;  // 找到碰撞體，立刻終止查詢
        }
        return true;  // 繼續檢查下一個 Fixture
    }
};

class DrawingRayCastCallback : public b2RayCastCallback {
   public:
    bool hit = false;
    b2Vec2 hitPoint;
    b2Body* ignoreBody = nullptr;

    float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& /*normal*/, float fraction) override {
        if (fixture->GetBody() == ignoreBody) {
            return -1.0f;  // 忽略這條線本身包含的所有 fixtures
        }
        if (fixture->IsSensor()) {
            return -1.0f;  // Sensor 不阻擋畫線
        }
        hit = true;
        hitPoint = point;
        return fraction;
    }
};

PhysicalWorld::PhysicalWorld(std::vector<std::shared_ptr<CompositeObject>> compositeObjects, PassCondition* passCondition)
    : m_b2World(b2Vec2(0.0f, -9.8f)),
      m_CompositeObject(std::move(compositeObjects)),
      m_PassCondition(passCondition) {
    for (auto& obj : m_CompositeObject) {
        obj->AttachToWorld(&m_b2World);
    }
    m_PassCondition->AttachToWorld(&m_b2World);
};

void PhysicalWorld::Start() {
    m_IsActive = true;
}
void PhysicalWorld::Stop() {
    m_IsActive = false;
}

void PhysicalWorld::DrawObject(glm::vec2 position) {
    // 無正在畫的物件則先建立
    if (m_LastDrawingObject == nullptr) {
        // 檢查點有沒有碰到其他東西
        PointQueryCallback callback;
        b2Vec2 point = GameWorld::PixelsToMeters(position);
        callback.testPoint = point;

        b2AABB aabb;
        b2Vec2 d(0.001f, 0.001f);
        aabb.lowerBound = point - d;
        aabb.upperBound = point + d;

        m_b2World.QueryAABB(&callback, aabb);
        if (callback.hit) {
            return;
        }

        // 建立新的物件
        m_LastDrawingObject = std::make_shared<DrawnObject>(position);
        m_DrawnObjects.push_back(m_LastDrawingObject);
        m_LastDrawingObject->AttachToWorld(&m_b2World);
    } else {
        // 檢查射線有沒有碰到其他東西
        auto p1 = m_LastDrawingObject->m_Points.back();
        auto p2 = position;
        if (glm::distance(p1, p2) < 2.0f) {
            return;
        }
        DrawingRayCastCallback callback;
        callback.ignoreBody = m_LastDrawingObject->GetBody();
        b2Vec2 startP = GameWorld::PixelsToMeters(p1);
        b2Vec2 endP = GameWorld::PixelsToMeters(p2);

        m_b2World.RayCast(&callback, startP, endP);
        if (callback.hit) {
            glm::vec2 hitPixel = GameWorld::MetersToPixels(callback.hitPoint);
            float dist = glm::distance(p1, hitPixel);
            if (dist > 10.0f) {
                glm::vec2 dir = (hitPixel - p1) / dist;
                hitPixel -= dir * 10.0f;
            } else {
                hitPixel = p1;
            }
            m_LastDrawingObject->DrawNextPoint(hitPixel);
            return;
        }

        // 繪製新的線段
        m_LastDrawingObject->DrawNextPoint(position);
    }
}

void PhysicalWorld::EndDrawing() {
    if (m_LastDrawingObject == nullptr) {
        return;
    }
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
    m_PassCondition->Update();
}

}  // namespace GameWorld