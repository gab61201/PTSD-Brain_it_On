#include "PhysicalObject.hpp"

#include <box2d/box2d.h>

#include "CoordinateHelper.hpp"
#include "PhysicsWorld.hpp"
#include "Util/Image.hpp"

// --- 工廠方法 ---

std::shared_ptr<PhysicalObject> PhysicalObject::CreateCircle(
    PhysicsWorld& world, glm::vec2 posPixels, glm::vec2 sizePixels, float rotationRadians, bool isDynamic) {
    auto obj = std::make_shared<PhysicalObject>();

    // 建立 Box2D 物理圓形
    float radiusPixels = sizePixels.x / 2.0F;
    obj->m_Body = world.CreateCircle(posPixels, radiusPixels, rotationRadians, isDynamic);

    // TODO: 待確認！！
    // 建立 PTSD 視覺物件
    obj->m_Visual = std::make_shared<Util::GameObject>();
    obj->m_Visual->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/white_circle.png"));
    obj->m_Visual->m_Transform.translation = posPixels;
    obj->m_Visual->m_Transform.rotation = rotationRadians;
    obj->m_Visual->m_Transform.scale = sizePixels / glm::vec2(250.0F);

    return obj;
}

std::shared_ptr<PhysicalObject> PhysicalObject::CreateRectangle(
    PhysicsWorld& world, glm::vec2 posPixels, glm::vec2 sizePixels, float rotationRadians, bool isDynamic) {
    auto obj = std::make_shared<PhysicalObject>();

    // 建立 Box2D 物理矩形（halfSize = sizePixels / 2）
    obj->m_Body = world.CreateBox(posPixels, sizePixels / 2.0F, rotationRadians, isDynamic);

    // TODO: 待確認！！
    // 建立 PTSD 視覺物件
    obj->m_Visual = std::make_shared<Util::GameObject>();
    obj->m_Visual->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/white_floor.png"));
    obj->m_Visual->m_Transform.translation = posPixels;
    obj->m_Visual->m_Transform.rotation = rotationRadians;
    obj->m_Visual->m_Transform.scale = sizePixels / glm::vec2(600.0F, 50.0F);

    return obj;
}

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
