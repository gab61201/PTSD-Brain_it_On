#include "GameWorld/CompoundObject.hpp"

#include <box2d/box2d.h>

#include "GameWorld/CoordinateHelper.hpp"
#include "GameWorld/Physics.hpp"
#include "Util/Image.hpp"

#define IMAGE_SIZE 417.0F

namespace GameWorld {

CompoundObject::CompoundObject(Physics& physics, const std::vector<BaseObject>& children) {
    if (children.empty()) return;

    // Use the first child's properties to determine the root body's initial state
    glm::vec2 rootPos = children[0].GetPosition();
    float rootRot = children[0].GetRotation();
    int b2Type = children[0].GetBody() ? children[0].GetBody()->GetType() : b2_dynamicBody;

    m_Body = physics.CreateEmptyBody(rootPos, rootRot, b2Type);

    m_Visual = std::make_shared<Util::GameObject>();
    m_Visual->m_Transform.translation = rootPos;
    m_Visual->m_Transform.rotation = rootRot;

    // Add each child as a fixture to the root body
    for (const auto& child : children) {
        glm::vec2 localOffset = child.GetPosition();  // Treat child's position as local offset
        float localRot = child.GetRotation();
        Shape shape = child.GetShape();
        glm::vec2 size = child.GetSize();
        bool isSensor = child.GetBody() ? child.GetBody()->GetFixtureList()->IsSensor() : false;

        std::string imagePath = "";

        if (shape == Shape::CIRCLE) {
            physics.AddCircleFixture(m_Body, localOffset, size.x / 2.0f, isSensor);
            imagePath = RESOURCE_DIR "/Images/circle.png";
        } else if (shape == Shape::RECTANGLE) {
            physics.AddBoxFixture(m_Body, localOffset, size, localRot, isSensor);
            imagePath = RESOURCE_DIR "/Images/square.png";
        }

        auto visual = std::make_shared<Util::GameObject>();
        if (!imagePath.empty()) {
            visual->SetDrawable(std::make_shared<Util::Image>(imagePath));
        }
        visual->m_Transform.scale = size / IMAGE_SIZE;

        m_Visual->AddChild(visual);
        visual->m_Transform.translation = localOffset;
        visual->m_Transform.rotation = localRot;

        m_ChildVisuals.push_back(visual);
        m_ChildOffsets.push_back(localOffset);
        m_ChildLocalRotations.push_back(localRot);
    }
}

void CompoundObject::Sync() {
    if (!m_Body) return;

    m_Visual->m_Transform.translation = MetersToPixels(m_Body->GetPosition());
    m_Visual->m_Transform.rotation = m_Body->GetAngle();

    for (size_t i = 0; i < m_ChildVisuals.size(); ++i) {
        if (!m_ChildVisuals[i]) continue;

        b2Vec2 localPosMeters = PixelsToMeters(m_ChildOffsets[i]);
        b2Vec2 globalPosMeters = m_Body->GetWorldPoint(localPosMeters);

        m_ChildVisuals[i]->m_Transform.translation = MetersToPixels(globalPosMeters);
        m_ChildVisuals[i]->m_Transform.rotation = m_Body->GetAngle() + m_ChildLocalRotations[i];
    }
}

std::vector<std::shared_ptr<Util::GameObject>> CompoundObject::GetVisuals() const {
    return m_ChildVisuals;  // Return child visuals if needed, or maybe the root visual
}

glm::vec2 CompoundObject::GetPosition() const {
    if (m_Body == nullptr) return {0.0F, 0.0F};
    return MetersToPixels(m_Body->GetPosition());
}

void CompoundObject::SetPosition(glm::vec2 posPixels) {
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

float CompoundObject::GetRotation() const {
    if (m_Body == nullptr) return 0.0F;
    return m_Body->GetAngle();
}

void CompoundObject::SetRotation(float angleRadians) {
    if (m_Body) {
        m_Body->SetTransform(m_Body->GetPosition(), angleRadians);
        m_Body->SetAwake(true);
    }
    if (m_Visual) {
        m_Visual->m_Transform.rotation = angleRadians;
    }
}

}  // namespace GameWorld
