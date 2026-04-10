#ifndef COMPOSITE_OBJECT_HPP
#define COMPOSITE_OBJECT_HPP

#include "Physics/Physics.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "GameWorld/BaseObject.hpp"
#include "Util/Renderer.hpp"

namespace GameWorld {

enum class BodyType {
    STATIC,
    DYNAMIC,
    KINEMATIC
};

class CompositeObject {
   public:
    // 傳入多個 BaseObject 作為子物件
    CompositeObject() = default;
    CompositeObject(
        std::vector<std::shared_ptr<BaseObject>> baseObjects,
        BodyType bodyType = BodyType::STATIC,
        glm::vec2 position = {0.0F, 0.0F},
        float rotation = 0.0F);

    ~CompositeObject() = default;

    virtual void Update();

    virtual void AttachToWorld(Physics::WorldPtr world);

   protected:
    Util::Renderer m_Renderer;
    std::vector<std::shared_ptr<BaseObject>> m_BaseObjects;
    BodyType m_BodyType;
    glm::vec2 m_Position;
    float m_Rotation;
    Physics::BodyPtr m_Body = b2_nullBodyId;
    // float m_Magnetism = 0;
};

}  // namespace GameWorld

#endif