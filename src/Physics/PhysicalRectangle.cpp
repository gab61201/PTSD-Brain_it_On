#include "Physics/PhysicalRectangle.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Util/Image.hpp"

#define IMAGE_SIZE 417.0F

std::shared_ptr<PhysicalRectangle> PhysicalRectangle::Create(
    PhysicsWorld& world, glm::vec2 posPixels, glm::vec2 sizePixels, float rotationRadians, bool isDynamic) {
    auto obj = std::make_shared<PhysicalRectangle>();

    // 建立 Box2D 物理矩形（halfSize = sizePixels / 2）
    obj->m_Body = world.CreateBox(posPixels, sizePixels / 2.0F, rotationRadians, isDynamic);

    // 建立 PTSD 視覺物件
    obj->m_Visual = std::make_shared<Util::GameObject>();
    obj->m_Visual->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/Images/square.png"));
    obj->m_Visual->m_Transform.translation = posPixels;
    obj->m_Visual->m_Transform.rotation = rotationRadians;
    obj->m_Visual->m_Transform.scale = sizePixels / IMAGE_SIZE;

    return obj;
}
