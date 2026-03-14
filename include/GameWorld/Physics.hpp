#ifndef GAMEWORLD_PHYSICS_HPP
#define GAMEWORLD_PHYSICS_HPP

#include <box2d/b2_world.h>
#include <glm/vec2.hpp>

class b2Body;

namespace GameWorld {

class Physics {
   public:
    Physics();
    ~Physics() = default;

    void Step();

    // 給 BaseObject 和 CompoundObject 使用
    b2Body* CreateEmptyBody(glm::vec2 posPixels, float rotationRadians, int bodyType);
    void AddCircleFixture(b2Body* body, glm::vec2 localOffsetPixels, float radiusPixels, bool isSensor);
    void AddBoxFixture(b2Body* body, glm::vec2 localOffsetPixels, glm::vec2 sizePixels, float localRotationRadians, bool isSensor);

   private:
    b2World m_World;
};

}  // namespace GameWorld

#endif  // GAMEWORLD_PHYSICS_HPP
