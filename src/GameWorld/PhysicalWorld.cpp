#include "GameWorld/PhysicalWorld.hpp"

#include <box2d/b2_distance.h>

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
    b2Body* m_CurrentBody = nullptr;  // 正在繪製的物體

    bool ReportFixture(b2Fixture* fixture) override {
        // 通常我們不把 Sensor 當作阻擋畫線的物體
        if (fixture->IsSensor()) return true;
        // 忽略正在繪製的物體本身，允許在同條線上反向繪製
        if (fixture->GetBody() == m_CurrentBody) return true;
        if (fixture->TestPoint(testPoint)) {
            hit = true;
            return false;  // 找到碰撞體，立刻終止查詢
        }
        return true;  // 繼續檢查下一個 Fixture
    }
};

class WorldCircleCastCallback : public b2QueryCallback {
   public:
    b2CircleShape m_circle;
    b2Transform m_circleStartTransform;
    b2Vec2 m_translation;  // 射線位移向量
    b2Body* m_CurrentBody;
    float m_closestFraction = 1.0f;  // 1.0 代表走完全程沒撞到
    bool m_Hit = false;

    // 建構子：接收圓形與發射資訊
    WorldCircleCastCallback(b2Body* currentBody, const b2CircleShape& circle, const b2Transform& start, const b2Vec2& trans)
        : m_CurrentBody(currentBody), m_circle(circle), m_circleStartTransform(start), m_translation(trans) {}

    // 當 Box2D 的 AABB 框找到可疑物件時，會自動呼叫這裡
    bool ReportFixture(b2Fixture* fixture) override {
        // 如果是感應區 (Sensor) 或不想碰撞的物體，可以直接跳過
        if (fixture->IsSensor() || fixture->GetBody() == m_CurrentBody) {
            return true;
        }

        b2ShapeCastInput input;
        input.proxyA.Set(fixture->GetShape(), 0);  // 遊戲世界裡的物件當 A (靜止)
        input.proxyB.Set(&m_circle, 0);            // 發射的圓形當 B (移動)
        input.transformA = fixture->GetBody()->GetTransform();
        input.transformB = m_circleStartTransform;
        input.translationB = m_translation;  // 只有 B 會移動

        b2ShapeCastOutput output;
        bool hit = b2ShapeCast(&output, &input);

        // 如果撞到了，而且比之前記錄的還要近，就更新最佳結果
        if (hit && output.lambda < m_closestFraction) {
            m_closestFraction = output.lambda;
            m_Hit = true;
        }
        // 回傳 true 代表「請繼續幫我找下一個可疑物件」，不要提早中斷 AABB 搜尋
        return true;
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
        callback.m_CurrentBody = nullptr;  // 初次繪製時沒有現存物體

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
        auto p1 = GameWorld::PixelsToMeters(m_LastDrawingObject->m_Points.back());
        auto p2 = GameWorld::PixelsToMeters(position);
        if (b2Distance(p1, p2) < GameWorld::PixelsToMeters(2.0f)) {
            return;
        }
        b2Vec2 translation = p2 - p1;
        // 2. 設定你要投射的圓形與初始位置
        b2CircleShape circle;
        circle.m_radius = GameWorld::PixelsToMeters(10.0f);  // 射線寬度
        b2Transform startTransform(p1, b2Rot(0.0f));

        // 3. 計算能包住整條射線軌跡的 AABB 框
        b2AABB aabb;
        b2Vec2 minPos = b2Min(p1, p2);
        b2Vec2 maxPos = b2Max(p1, p2);

        // ⭐️ 關鍵：框框必須向外擴張圓形的半徑，否則會漏抓邊緣的物體
        aabb.lowerBound = minPos - b2Vec2(circle.m_radius, circle.m_radius);
        aabb.upperBound = maxPos + b2Vec2(circle.m_radius, circle.m_radius);

        // 4. 開始大範圍搜尋與精細測試
        WorldCircleCastCallback callback(m_LastDrawingObject->m_Body, circle, startTransform, translation);
        m_b2World.QueryAABB(&callback, aabb);  // world 是你的 b2World 指標

        // 5. 檢視最終結果
        // callback.m_closestFixture 就是你真正打到的物體
        // 繪製新的線段
        if (callback.m_Hit) {
            b2Vec2 rayDir = translation;
            rayDir.Normalize();
            // 安全距離設為圓形半徑的 0.3 倍，讓線段更接近物體
            b2Vec2 centerAtHit = p1 + callback.m_closestFraction * translation - (circle.m_radius * 0.5f * rayDir);
            m_LastDrawingObject->DrawNextPoint(GameWorld::MetersToPixels(centerAtHit));
        } else {
            m_LastDrawingObject->DrawNextPoint(position);
        }
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
