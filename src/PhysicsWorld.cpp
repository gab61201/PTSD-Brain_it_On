#include "PhysicsWorld.hpp"

#include <box2d/box2d.h>

namespace {
constexpr float kPixelsPerMeter = 50.0F;
constexpr float kTimeStep = 1.0F / 60.0F;
constexpr int kVelocityIterations = 8;
constexpr int kPositionIterations = 3;

// --- Coordinate conversion helpers ---

b2Vec2 PixelsToMeters(glm::vec2 pixels) {
    return {pixels.x / kPixelsPerMeter, pixels.y / kPixelsPerMeter};
}

glm::vec2 MetersToPixels(b2Vec2 meters) {
    return {meters.x * kPixelsPerMeter, meters.y * kPixelsPerMeter};
}

float PixelsToMeters(float pixels) { return pixels / kPixelsPerMeter; }
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
