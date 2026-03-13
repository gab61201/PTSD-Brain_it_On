#include "Physics/PhysicsWorld.hpp"

#include <box2d/box2d.h>

#include "Physics/CoordinateHelper.hpp"

namespace {
constexpr float kTimeStep = 1.0F / 60.0F;
constexpr int kVelocityIterations = 8;
constexpr int kPositionIterations = 3;
}  // namespace

struct PhysicsWorld::Impl {
    b2World world{b2Vec2(0.0F, -9.8F)};
    b2Body* circleBody = nullptr;
    glm::vec2 circleInitialPixelPos = {0.0F, 0.0F};
};

PhysicsWorld::PhysicsWorld() : m_Impl(std::make_unique<Impl>()) {}

PhysicsWorld::~PhysicsWorld() = default;

PhysicsWorld::PhysicsWorld(PhysicsWorld&&) noexcept = default;

PhysicsWorld& PhysicsWorld::operator=(PhysicsWorld&&) noexcept = default;

// Steps to create a physics object
// b2BodyDef -> CreateBody -> b2Shape -> b2FixtureDef -> CreateFixture

void PhysicsWorld::InitializeScene() {
    // Create ground (static body)
    // Position in PTSD pixel coordinates: (0, -225)
    constexpr glm::vec2 groundPixelPos = {0.0F, -225.0F};
    constexpr glm::vec2 groundHalfSizePixels = {600.0F, 25.0F};

    b2BodyDef groundBodyDef;
    groundBodyDef.position = PixelsToMeters(groundPixelPos);
    groundBodyDef.angle = 0.1F;
    auto* groundBody = m_Impl->world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(PixelsToMeters(groundHalfSizePixels.x),
                       PixelsToMeters(groundHalfSizePixels.y));
    groundBody->CreateFixture(&groundBox, 0.0F);

    // Create circle (dynamic body)
    // Position in PTSD pixel coordinates: (0, 200)
    constexpr glm::vec2 circlePixelPos = {0.0F, 200.0F};
    m_Impl->circleInitialPixelPos = circlePixelPos;

    b2BodyDef circleBodyDef;
    circleBodyDef.type = b2_dynamicBody;
    circleBodyDef.position = PixelsToMeters(circlePixelPos);
    m_Impl->circleBody = m_Impl->world.CreateBody(&circleBodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = PixelsToMeters(25.0F);

    b2FixtureDef circleFixtureDef;
    circleFixtureDef.shape = &circleShape;
    circleFixtureDef.density = 1.0F;
    circleFixtureDef.friction = 0.3F;
    circleFixtureDef.restitution = 0.5F;
    m_Impl->circleBody->CreateFixture(&circleFixtureDef);
}

void PhysicsWorld::Step() {
    m_Impl->world.Step(kTimeStep, kVelocityIterations, kPositionIterations);
}

glm::vec2 PhysicsWorld::GetCirclePositionPixels() const {
    if (m_Impl->circleBody == nullptr) {
        return {0.0F, 0.0F};
    }

    return MetersToPixels(m_Impl->circleBody->GetPosition());
}

void PhysicsWorld::SetCirclePosition(glm::vec2 pixelPos) {
    if (m_Impl->circleBody) {
        m_Impl->circleBody->SetTransform(PixelsToMeters(pixelPos), 0.0F);
        m_Impl->circleBody->SetLinearVelocity(b2Vec2(0.0F, 0.0F));
        m_Impl->circleBody->SetAngularVelocity(0.0F);
        m_Impl->circleBody->SetAwake(true);
    }
}

void PhysicsWorld::ResetCircle() {
    SetCirclePosition(m_Impl->circleInitialPixelPos);
}

float PhysicsWorld::GetCircleRotationRadians() const {
    if (m_Impl->circleBody == nullptr) {
        return 0.0F;
    }

    return m_Impl->circleBody->GetAngle();
}

b2Body* PhysicsWorld::CreateCircle(
    glm::vec2 posPixels, float radiusPixels, float rotationRadians, bool isDynamic) {
    b2BodyDef bodyDef;
    bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
    bodyDef.position = PixelsToMeters(posPixels);
    bodyDef.angle = rotationRadians;
    auto* body = m_Impl->world.CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = PixelsToMeters(radiusPixels);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0F;
    fixtureDef.friction = 0.3F;
    fixtureDef.restitution = 0.5F;
    body->CreateFixture(&fixtureDef);

    return body;
}

b2Body* PhysicsWorld::CreateBox(
    glm::vec2 posPixels, glm::vec2 halfSizePixels, float rotationRadians, bool isDynamic) {
    b2BodyDef bodyDef;
    bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
    bodyDef.position = PixelsToMeters(posPixels);
    bodyDef.angle = rotationRadians;
    auto* body = m_Impl->world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(PixelsToMeters(halfSizePixels.x), PixelsToMeters(halfSizePixels.y));

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0F;
    fixtureDef.friction = 0.3F;
    fixtureDef.restitution = 0.5F;
    body->CreateFixture(&fixtureDef);

    return body;
}

// --- Compound body 用 ---

b2Body* PhysicsWorld::CreateEmptyBody(
    glm::vec2 posPixels, float rotationRadians, bool isDynamic) {
    b2BodyDef bodyDef;
    bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
    bodyDef.position = PixelsToMeters(posPixels);
    bodyDef.angle = rotationRadians;
    return m_Impl->world.CreateBody(&bodyDef);
}

void PhysicsWorld::AddCircleFixture(
    b2Body* body, glm::vec2 localOffsetPixels, float radiusPixels) {
    b2CircleShape shape;
    shape.m_radius = PixelsToMeters(radiusPixels);
    shape.m_p = PixelsToMeters(localOffsetPixels);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0F;
    fixtureDef.friction = 0.3F;
    fixtureDef.restitution = 0.5F;
    body->CreateFixture(&fixtureDef);
}

void PhysicsWorld::AddBoxFixture(
    b2Body* body, glm::vec2 localOffsetPixels, glm::vec2 halfSizePixels, float localRotation) {
    b2PolygonShape shape;
    b2Vec2 center = PixelsToMeters(localOffsetPixels);
    shape.SetAsBox(
        PixelsToMeters(halfSizePixels.x),
        PixelsToMeters(halfSizePixels.y),
        center,
        localRotation);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0F;
    fixtureDef.friction = 0.3F;
    fixtureDef.restitution = 0.5F;
    body->CreateFixture(&fixtureDef);
}

