#include "GameWorld/CompositeObject/MagnetObject.hpp"

#include <cmath>

#include "GameWorld/CoordinateHelper.hpp"

namespace GameWorld {

namespace {

constexpr float kMaxForce = 500.0F;
constexpr float kMinDistance = 10.0F;       // pixels
constexpr float kInfluenceRadius = 200.0F;  // pixels

struct MagnetQueryContext {
    b2BodyId magnetBodyId = b2_nullBodyId;
    float magnetism = 0.0F;
    b2Vec2 magnetPosition = {0.0F, 0.0F};
};

bool MagnetQueryCallback(b2ShapeId shapeId, void* context) {
    auto* ctx = static_cast<MagnetQueryContext*>(context);
    b2BodyId bodyId = b2Shape_GetBody(shapeId);

    if (B2_ID_EQUALS(bodyId, ctx->magnetBodyId)) {
        return true;
    }

    if (b2Shape_IsSensor(shapeId)) {
        return true;
    }

    b2Vec2 targetPos = b2Body_GetPosition(bodyId);
    b2Vec2 delta = {targetPos.x - ctx->magnetPosition.x, targetPos.y - ctx->magnetPosition.y};
    float distance = b2Length(delta);

    float minDistMeters = PixelsToMeters(kMinDistance);
    if (distance < minDistMeters) {
        distance = minDistMeters;
    }

    float otherMagnetism = 0.0F;
    void* userData = b2Body_GetUserData(bodyId);
    if (userData != nullptr) {
        auto* otherMagnet = static_cast<MagnetObject*>(userData);
        otherMagnetism = otherMagnet->GetMagnetism();
    }

    if (otherMagnetism == 0.0F) {
        return true;
    }

    float forceMag;
    float product = ctx->magnetism * otherMagnetism;
    forceMag = -product / (distance * distance);

    float maxForceMeters = PixelsToMeters(kMaxForce);
    if (forceMag > maxForceMeters) {
        forceMag = maxForceMeters;
    }
    if (forceMag < -maxForceMeters) {
        forceMag = -maxForceMeters;
    }

    b2Vec2 direction;
    if (distance > 0.0001F) {
        direction = {-delta.x / distance, -delta.y / distance};
    } else {
        direction = {0.0F, 0.0F};
    }

    b2Vec2 force = {direction.x * forceMag, direction.y * forceMag};
    b2Body_ApplyForceToCenter(bodyId, force, true);

    return true;
}

}  // namespace

MagnetObject::MagnetObject(
    std::vector<std::shared_ptr<Shape>> shapes,
    BodyType bodyType,
    glm::vec2 position,
    float rotation,
    float magnetism)
    : CompositeObject(std::move(shapes), bodyType, position, rotation),
      m_Magnetism(magnetism) {}

void MagnetObject::AttachToWorld(b2WorldId world) {
    CompositeObject::AttachToWorld(world);
    m_WorldId = world;
    b2Body_SetUserData(m_b2BodyId, this);
}

void MagnetObject::Update() {
    if (B2_IS_NON_NULL(m_WorldId) && B2_IS_NON_NULL(m_b2BodyId) && m_Magnetism != 0.0F) {
        b2Vec2 pos = b2Body_GetPosition(m_b2BodyId);
        float radius = PixelsToMeters(kInfluenceRadius);

        b2AABB aabb;
        aabb.lowerBound = {pos.x - radius, pos.y - radius};
        aabb.upperBound = {pos.x + radius, pos.y + radius};

        MagnetQueryContext ctx;
        ctx.magnetBodyId = m_b2BodyId;
        ctx.magnetism = m_Magnetism;
        ctx.magnetPosition = pos;

        b2QueryFilter filter = b2DefaultQueryFilter();
        b2World_OverlapAABB(m_WorldId, aabb, filter, MagnetQueryCallback, &ctx);
    }

    CompositeObject::Update();
}

}  // namespace GameWorld
