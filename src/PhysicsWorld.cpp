#include "PhysicsWorld.hpp"

#include <box2d/box2d.h>

namespace {
constexpr float kPixelsPerMeter = 50.0F;
constexpr float kTimeStep = 1.0F / 60.0F;
constexpr int kVelocityIterations = 8;
constexpr int kPositionIterations = 3;
}  // namespace

struct PhysicsWorld::Impl {
    b2World world{b2Vec2(0.0F, -9.8F)};
    b2Body* circleBody = nullptr;
};

PhysicsWorld::PhysicsWorld() : m_Impl(std::make_unique<Impl>()) {}

PhysicsWorld::~PhysicsWorld() = default;

PhysicsWorld::PhysicsWorld(PhysicsWorld&&) noexcept = default;

PhysicsWorld& PhysicsWorld::operator=(PhysicsWorld&&) noexcept = default;

// Steps to create a physics object
// b2BodyDef -> CreateBody -> b2Shape -> b2FixtureDef -> CreateFixture

void PhysicsWorld::InitializeScene() {
    // Create ground (static body)
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0F, -4.5F);
    groundBodyDef.angle = 0.1F;
    auto* groundBody = m_Impl->world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(12.0F, 0.5F);
    groundBody->CreateFixture(&groundBox, 0.0F);

    // Create circle (dynamic body)
    b2BodyDef circleBodyDef;
    circleBodyDef.type = b2_dynamicBody;
    circleBodyDef.position.Set(0.0F, 4.0F);
    m_Impl->circleBody = m_Impl->world.CreateBody(&circleBodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = 0.5F;

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

    const auto position = m_Impl->circleBody->GetPosition();
    return {position.x * kPixelsPerMeter, position.y * kPixelsPerMeter};
}

void PhysicsWorld::ResetCircle() {
    if (m_Impl->circleBody) {
        m_Impl->circleBody->SetTransform(b2Vec2(0.0F, 4.0F), 0.0F);
        m_Impl->circleBody->SetLinearVelocity(b2Vec2(0.0F, 0.0F));
        m_Impl->circleBody->SetAngularVelocity(0.0F);
        m_Impl->circleBody->SetAwake(true);
    }
}

float PhysicsWorld::GetCircleRotationRadians() const {
    if (m_Impl->circleBody == nullptr) {
        return 0.0F;
    }

    return m_Impl->circleBody->GetAngle();
}
