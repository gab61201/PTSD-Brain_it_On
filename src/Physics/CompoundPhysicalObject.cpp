#include "Physics/CompoundPhysicalObject.hpp"
#include <box2d/box2d.h>

#include "Physics/CoordinateHelper.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Util/Image.hpp"

#define IMAGE_SIZE 417.0F

std::shared_ptr<CompoundPhysicalObject> CompoundPhysicalObject::Create(
    PhysicsWorld& world, glm::vec2 posPixels, float rotationRadians, bool isDynamic) {
    auto obj = std::make_shared<CompoundPhysicalObject>();

    // 建立沒有任何碰撞體的空 body
    obj->m_Body = world.CreateEmptyBody(posPixels, rotationRadians, isDynamic);

    // 建立空的 root visual
    obj->m_Visual = std::make_shared<Util::GameObject>();
    obj->m_Visual->m_Transform.translation = posPixels;
    obj->m_Visual->m_Transform.rotation = rotationRadians;

    return obj;
}

void CompoundPhysicalObject::AddCircle(
    PhysicsWorld& world, glm::vec2 localOffsetPixels, float radiusPixels, const std::string& imagePath) {
    if (!m_Body) return;

    world.AddCircleFixture(m_Body, localOffsetPixels, radiusPixels);

    auto visual = std::make_shared<Util::GameObject>();
    visual->SetDrawable(std::make_shared<Util::Image>(imagePath));
    glm::vec2 sizePixels(radiusPixels * 2.0f, radiusPixels * 2.0f);
    visual->m_Transform.scale = sizePixels / IMAGE_SIZE;
    
    m_Visual->AddChild(visual);
    visual->m_Transform.translation = localOffsetPixels; // Set local translation relative to parent
    
    // Store if we need manual sync instead of hierarchy (if PTSD GameObject AddChild doesn't auto-sync transforms globally)
    m_ChildVisuals.push_back(visual);
    m_ChildOffsets.push_back(localOffsetPixels);
    m_ChildLocalRotations.push_back(0.0f); // Circle has no meaningful rotation visually, but default to 0
}

void CompoundPhysicalObject::AddRectangle(
    PhysicsWorld& world, glm::vec2 localOffsetPixels, glm::vec2 sizePixels, float localRotationRadians, const std::string& imagePath) {
    if (!m_Body) return;

    world.AddBoxFixture(m_Body, localOffsetPixels, sizePixels / 2.0F, localRotationRadians);

    auto visual = std::make_shared<Util::GameObject>();
    visual->SetDrawable(std::make_shared<Util::Image>(imagePath));
    visual->m_Transform.scale = sizePixels / IMAGE_SIZE;
    visual->m_Transform.rotation = localRotationRadians;
    
    m_Visual->AddChild(visual);
    visual->m_Transform.translation = localOffsetPixels;
    
    m_ChildVisuals.push_back(visual);
    m_ChildOffsets.push_back(localOffsetPixels);
    m_ChildLocalRotations.push_back(localRotationRadians);
}

void CompoundPhysicalObject::Sync() {
    PhysicalObject::Sync(); // 同步 root 位置
    
    if (!m_Body) return;

    // 因為 PTSD 的 Renderer 目前不支援父子階層 (Hierarchy) 的 Transform 自動繼承，
    // 所以我們必須手動將 Box2D 算出來的 global 座標與旋轉，套用給每個子視覺物件。
    for (size_t i = 0; i < m_ChildVisuals.size(); ++i) {
        if (!m_ChildVisuals[i]) continue;

        // 計算 Global Position: body->GetWorldPoint(localPoint)
        b2Vec2 localPosMeters = PixelsToMeters(m_ChildOffsets[i]);
        b2Vec2 globalPosMeters = m_Body->GetWorldPoint(localPosMeters);
        
        m_ChildVisuals[i]->m_Transform.translation = MetersToPixels(globalPosMeters);
        m_ChildVisuals[i]->m_Transform.rotation = m_Body->GetAngle() + m_ChildLocalRotations[i];
    }
}
