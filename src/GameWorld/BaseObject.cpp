#include "GameWorld/BaseObject.hpp"

#include <box2d/box2d.h>

#include "GameWorld/CoordinateHelper.hpp"
#include "GameWorld/Physics.hpp"
#include "Util/Image.hpp"

#define IMAGE_SIZE 417.0F

namespace GameWorld {

BaseObject::BaseObject(
    Physics& physics,
    Shape shape,
    glm::vec2 size,
    glm::vec2 position,
    float rotation,
    BodyType bodyType,
    bool isSensor)
    : m_Shape(shape), m_Size(size) {
    // Convert BodyType enum to Box2D integer type
    int b2Type = b2_staticBody;
    if (bodyType == BodyType::DYNAMIC)
        b2Type = b2_dynamicBody;
    else if (bodyType == BodyType::KINEMATIC)
        b2Type = b2_kinematicBody;

    m_Body = physics.CreateEmptyBody(position, rotation, b2Type);

    std::string imagePath = "";
    if (shape == Shape::CIRCLE) {
        physics.AddCircleFixture(m_Body, glm::vec2(0.0f, 0.0f), size.x / 2.0f, isSensor);
        imagePath = RESOURCE_DIR "/Images/circle.png";
    } else if (shape == Shape::RECTANGLE) {
        physics.AddBoxFixture(m_Body, glm::vec2(0.0f, 0.0f), size, 0.0f, isSensor);
        imagePath = RESOURCE_DIR "/Images/square.png";
    } else if (shape == Shape::TRIANGLE) {
        // TBD
    }

    m_Visual = std::make_shared<Util::GameObject>();
    if (!imagePath.empty()) {
        m_Visual->SetDrawable(std::make_shared<Util::Image>(imagePath));
    }
    m_Visual->m_Transform.scale = size / IMAGE_SIZE;
    m_Visual->m_Transform.translation = position;
    m_Visual->m_Transform.rotation = rotation;
}

void BaseObject::Sync() {
    if (m_Body == nullptr || m_Visual == nullptr) {
        return;
    }

    m_Visual->m_Transform.translation = MetersToPixels(m_Body->GetPosition());
    m_Visual->m_Transform.rotation = m_Body->GetAngle();
}

std::shared_ptr<Util::GameObject> BaseObject::GetVisual() const {
    return m_Visual;
}

glm::vec2 BaseObject::GetPosition() const {
    if (m_Body == nullptr) {
        return {0.0F, 0.0F};
    }
    return MetersToPixels(m_Body->GetPosition());
}

void BaseObject::SetPosition(glm::vec2 posPixels) {
    if (m_Body) {
        b2Vec2 meters = PixelsToMeters(posPixels);
        m_Body->SetTransform(meters, m_Body->GetAngle());
        m_Body->SetLinearVelocity(b2Vec2(0.0F, 0.0F));
        m_Body->SetAngularVelocity(0.0F);
        m_Body->SetAwake(true);
    }
    if (m_Visual) {
        m_Visual->m_Transform.translation = posPixels;
    }
}

float BaseObject::GetRotation() const {
    if (m_Body == nullptr) {
        return 0.0F;
    }
    return m_Body->GetAngle();
}

void BaseObject::SetRotation(float angleRadians) {
    if (m_Body) {
        m_Body->SetTransform(m_Body->GetPosition(), angleRadians);
        m_Body->SetAwake(true);
    }
    if (m_Visual) {
        m_Visual->m_Transform.rotation = angleRadians;
    }
}

}  // namespace GameWorld
