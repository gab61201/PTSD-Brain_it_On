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
    float fraction = 1.0f;
    b2BodyId ignoreBody = b2_nullBodyId;
    b2Vec2 normal = {0.0f, 0.0f};
};

float ReportDrawingShape(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context) {
    auto* castContext = static_cast<DrawingRayCastContext*>(context);

    if (B2_IS_NON_NULL(castContext->ignoreBody) && B2_ID_EQUALS(b2Shape_GetBody(shapeId), castContext->ignoreBody)) {
        return -1.0f;
    }
    if (b2Shape_IsSensor(shapeId)) {
        return -1.0f;
    }

    if (fraction < castContext->fraction) {
        castContext->hit = true;
        castContext->hitPoint = point;
        castContext->fraction = fraction;
        castContext->normal = normal;
    }
    return fraction;
}

}  // namespace

PhysicalWorld::PhysicalWorld(std::vector<std::shared_ptr<CompositeObject>> compositeObjects)
    : m_b2WorldId(b2_nullWorldId),
      m_CompositeObject(std::move(compositeObjects)) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, -9.8f};
    m_b2WorldId = b2CreateWorld(&worldDef);

    for (auto& obj : m_CompositeObject) {
        obj->AttachToWorld(m_b2WorldId);
    }
}

PhysicalWorld::~PhysicalWorld() {
    if (B2_IS_NON_NULL(m_b2WorldId)) {
        b2DestroyWorld(m_b2WorldId);
        m_b2WorldId = b2_nullWorldId;
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

        b2World_OverlapAABB(m_b2WorldId, aabb, filter, ReportPointShape, &callback);
        if (callback.hit) {
            return;
        }

        // 建立新的物件
        m_LastDrawingObject = std::make_shared<DrawnObject>(position);
        m_DrawnObjects.push_back(m_LastDrawingObject);
        m_LastDrawingObject->AttachToWorld(m_b2WorldId);
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
        b2ShapeProxy circleProxy = {{startP}, 1, GameWorld::PixelsToMeters(STROKE_WIDTH * 0.5F)};

        b2World_CastShape(m_b2WorldId, &circleProxy, translation, filter, ReportDrawingShape, &callback);
        if (callback.hit) {
            b2Vec2 centerAtHit = b2Add(startP, b2MulSV(callback.fraction, translation));
            b2Vec2 nextPoint = b2Add(centerAtHit, b2MulSV(GameWorld::PixelsToMeters(STROKE_WIDTH * 0.1F), callback.normal));
            m_LastDrawingObject->DrawNextPoint(GameWorld::MetersToPixels(nextPoint));
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

// ==========================================
// 每一幀的更新 (Update) - 遊戲主迴圈會呼叫這裡
// ==========================================
void PhysicalWorld::Update() {
    if (m_IsActive) {
        b2World_Step(m_b2WorldId, (1.0f / 60.0f), 4);
    }
    for (auto& obj : m_CompositeObject) {
        obj->Update();
    }
    for (auto& obj : m_DrawnObjects) {
        obj->Update();
    }
}

b2ContactEvents PhysicalWorld::GetContactEvents() {
    return b2World_GetContactEvents(m_b2WorldId);
}

}  // namespace GameWorld
