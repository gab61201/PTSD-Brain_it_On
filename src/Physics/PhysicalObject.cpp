#include "Physics/PhysicalObject.hpp"
#include <box2d/box2d.h>

#include "Physics/CoordinateHelper.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Util/Image.hpp"

#define IMAGE_SIZE 417.0F
// 工廠方法已移至各個子類別 (PhysicalCircle, PhysicalRectangle 等) 中

// --- 每幀同步 ---

void PhysicalObject::Sync() {
    if (m_Body == nullptr || m_Visual == nullptr) {
        return;
    }

    // Box2D 座標（公尺）→ PTSD 座標（像素）
    m_Visual->m_Transform.translation = MetersToPixels(m_Body->GetPosition());
    m_Visual->m_Transform.rotation = m_Body->GetAngle();
}

// --- Getter / Setter ---

std::shared_ptr<Util::GameObject> PhysicalObject::GetVisual() const {
    return m_Visual;
}

glm::vec2 PhysicalObject::GetPosition() const {
    // 以 Box2D 的位置為準
    if (m_Body == nullptr) {
        return {0.0F, 0.0F};
    }
    return MetersToPixels(m_Body->GetPosition());
}

void PhysicalObject::SetPosition(glm::vec2 posPixels) {
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

float PhysicalObject::GetRotation() const {
    if (m_Body == nullptr) {
        return 0.0F;
    }
    return m_Body->GetAngle();
}

void PhysicalObject::SetRotation(float angleRadians) {
    if (m_Body) {
        m_Body->SetTransform(m_Body->GetPosition(), angleRadians);
        m_Body->SetAwake(true);
    }
    if (m_Visual) {
        m_Visual->m_Transform.rotation = angleRadians;
    }
}
