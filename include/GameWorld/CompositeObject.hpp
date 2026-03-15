#ifndef COMPOSITE_OBJECT_HPP
#define COMPOSITE_OBJECT_HPP

#include <box2d/b2_world.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "GameWorld/BaseObject.hpp"

enum class BodyType {
    STATIC,
    DYNAMIC,
    KINEMATIC
};

class CompositeObject {
   public:
    // 傳入多個 BaseObject 作為子物件
    CompositeObject(
        std::vector<std::shared_ptr<BaseObject>> baseObjects,
        BodyType bodyType = BodyType::STATIC,
        glm::vec2 position = {0.0F, 0.0F},
        float rotation = 0.0F);

    ~CompositeObject() = default;

    void Update();

    void AttachToWorld(b2World* world);

   private:
    std::vector<std::shared_ptr<BaseObject>> m_BaseObjects;
    b2Body* m_Body = nullptr;
    BodyType m_BodyType;
    glm::vec2 m_Position;
    float m_Rotation;
};

#endif