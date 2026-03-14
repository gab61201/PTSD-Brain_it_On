#include "GameWorld/Physics.hpp"

#include <box2d/box2d.h>

#include "GameWorld/CoordinateHelper.hpp"

namespace {
constexpr float kTimeStep = 1.0F / 60.0F;
constexpr int kVelocityIterations = 8;
constexpr int kPositionIterations = 3;
}  // namespace

namespace GameWorld {

Physics::Physics() : m_World(b2Vec2(0.0F, -9.8F)) {}

void Physics::Step() {
    m_World.Step(kTimeStep, kVelocityIterations, kPositionIterations);
}

b2Body* Physics::CreateEmptyBody(
    glm::vec2 posPixels, float rotationRadians, int bodyType) {
    b2BodyDef bodyDef;
    bodyDef.type = static_cast<b2BodyType>(bodyType);
    bodyDef.position = PixelsToMeters(posPixels);
    bodyDef.angle = rotationRadians;
    return m_World.CreateBody(&bodyDef);
}

void Physics::AddCircleFixture(
    b2Body* body, glm::vec2 localOffsetPixels, float radiusPixels, bool isSensor) {
    b2CircleShape shape;
    shape.m_radius = PixelsToMeters(radiusPixels);
    shape.m_p = PixelsToMeters(localOffsetPixels);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0F;
    fixtureDef.friction = 0.3F;
    fixtureDef.restitution = 0.5F;
    fixtureDef.isSensor = isSensor;
    body->CreateFixture(&fixtureDef);
}

void Physics::AddBoxFixture(
    b2Body* body, glm::vec2 localOffsetPixels, glm::vec2 sizePixels, float localRotationRadians, bool isSensor) {
    b2PolygonShape shape;
    b2Vec2 center = PixelsToMeters(localOffsetPixels);
    shape.SetAsBox(
        PixelsToMeters(sizePixels.x / 2.0F),
        PixelsToMeters(sizePixels.y / 2.0F),
        center,
        localRotationRadians);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0F;
    fixtureDef.friction = 0.3F;
    fixtureDef.restitution = 0.5F;
    fixtureDef.isSensor = isSensor;
    body->CreateFixture(&fixtureDef);
}

}  // namespace GameWorld
