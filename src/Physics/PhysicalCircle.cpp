#include "Physics/PhysicalCircle.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Util/Image.hpp"

#define IMAGE_SIZE 417.0F

std::shared_ptr<PhysicalCircle> PhysicalCircle::Create(
    PhysicsWorld& world, glm::vec2 posPixels, float radiusPixels, float rotationRadians, bool isDynamic) {
    auto obj = std::make_shared<PhysicalCircle>();

    // 建立 Box2D 物理圓形
    obj->m_Body = world.CreateCircle(posPixels, radiusPixels, rotationRadians, isDynamic);

    // 建立 PTSD 視覺物件
    obj->m_Visual = std::make_shared<Util::GameObject>();
    obj->m_Visual->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/Images/circle.png"));
    obj->m_Visual->m_Transform.translation = posPixels;
    obj->m_Visual->m_Transform.rotation = rotationRadians;
    
    // Scale equals diameter / IMAGE_SIZE
    glm::vec2 sizePixels(radiusPixels * 2.0f, radiusPixels * 2.0f);
    obj->m_Visual->m_Transform.scale = sizePixels / IMAGE_SIZE;

    return obj;
}
