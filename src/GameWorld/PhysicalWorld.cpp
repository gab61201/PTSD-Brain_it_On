#include "GameWorld/PhysicalWorld.hpp"

#include "GameWorld/BaseObject.hpp"
#include "GameWorld/CoordinateHelper.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

#define STROKE_WIDTH 10.0F

namespace GameWorld {

namespace {

struct PointQueryContext {
    bool hit = false;
    b2Vec2 testPoint = {0.0f, 0.0f};
};

bool ReportPointShape(b2ShapeId shapeId, void* context) {
    auto* queryContext = static_cast<PointQueryContext*>(context);
    if (b2Shape_IsSensor(shapeId)) {
        return true;
    }
    if (b2Shape_TestPoint(shapeId, queryContext->testPoint)) {
        queryContext->hit = true;
        return false;
    }
    return true;
}

struct DrawingRayCastContext {
    bool hit = false;
    b2Vec2 hitPoint = {0.0f, 0.0f};
    b2BodyId ignoreBody = b2_nullBodyId;
};

float ReportDrawingShape(b2ShapeId shapeId, b2Vec2 point, b2Vec2 /*normal*/, float fraction, void* context) {
    auto* rayContext = static_cast<DrawingRayCastContext*>(context);

    if (B2_IS_NON_NULL(rayContext->ignoreBody) && B2_ID_EQUALS(b2Shape_GetBody(shapeId), rayContext->ignoreBody)) {
        return -1.0f;
    }
    if (b2Shape_IsSensor(shapeId)) {
        return -1.0f;
    }

    rayContext->hit = true;
    rayContext->hitPoint = point;
    return fraction;
}

}  // namespace

PhysicalWorld::PhysicalWorld(std::vector<std::shared_ptr<CompositeObject>> compositeObjects, PassCondition* passCondition)
    : m_b2World(b2_nullWorldId),
      m_CompositeObject(std::move(compositeObjects)),
      m_PassCondition(passCondition) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, -9.8f};
    m_b2World = b2CreateWorld(&worldDef);

    for (auto& obj : m_CompositeObject) {
        obj->AttachToWorld(m_b2World);
    }
    if (m_PassCondition != nullptr) {
        m_PassCondition->AttachToWorld(m_b2World);
    }
}

PhysicalWorld::~PhysicalWorld() {
    if (B2_IS_NON_NULL(m_b2World)) {
        b2DestroyWorld(m_b2World);
        m_b2World = b2_nullWorldId;
    }
}

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
        PointQueryContext callback;
        b2Vec2 point = GameWorld::PixelsToMeters(position);
        callback.testPoint = point;

        b2AABB aabb;
        b2Vec2 d = {0.001f, 0.001f};
        aabb.lowerBound = {point.x - d.x, point.y - d.y};
        aabb.upperBound = {point.x + d.x, point.y + d.y};

        b2QueryFilter filter = b2DefaultQueryFilter();

        b2World_OverlapAABB(m_b2World, aabb, filter, ReportPointShape, &callback);
        if (callback.hit) {
            return;
        }

        // 建立新的物件
        m_LastDrawingObject = std::make_shared<DrawnObject>(position);
        m_DrawnObjects.push_back(m_LastDrawingObject);
        m_LastDrawingObject->AttachToWorld(m_b2World);
    } else {
        // 檢查射線有沒有碰到其他東西
        auto p1 = m_LastDrawingObject->m_Points.back();
        auto p2 = position;
        if (glm::distance(p1, p2) < 2.0f) {
            return;
        }
        DrawingRayCastContext callback;
        callback.ignoreBody = m_LastDrawingObject->Getb2BodyId();
        b2Vec2 startP = GameWorld::PixelsToMeters(p1);
        b2Vec2 endP = GameWorld::PixelsToMeters(p2);
        b2Vec2 translation = {endP.x - startP.x, endP.y - startP.y};

        b2QueryFilter filter = b2DefaultQueryFilter();

        b2World_CastRay(m_b2World, startP, translation, filter, ReportDrawingShape, &callback);
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
    return m_PassCondition != nullptr ? m_PassCondition->Check() : false;
}
// ==========================================
// 每一幀的更新 (Update) - 遊戲主迴圈會呼叫這裡
// ==========================================
void PhysicalWorld::Update() {
    b2World_Step(m_b2World, m_IsActive ? (1.0f / 60.0f) : 0.0f, 4);

    if (m_PassCondition != nullptr) {
        m_PassCondition->ConsumeContactEvents(m_b2World);
    }

    for (auto& obj : m_CompositeObject) {
        obj->Update();
    }
    for (auto& obj : m_DrawnObjects) {
        obj->Update();
    }

    if (m_PassCondition != nullptr) {
        m_PassCondition->Update();
    }
}

}  // namespace GameWorld
