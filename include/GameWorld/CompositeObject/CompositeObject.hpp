#ifndef COMPOSITE_OBJECT_HPP
#define COMPOSITE_OBJECT_HPP

#include <box2d/box2d.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "GameWorld/Shape/Shape.hpp"
#include "Util/Renderer.hpp"

namespace GameWorld {

enum class BodyType {
    STATIC = b2_staticBody,
    KINEMATIC = b2_kinematicBody,
    DYNAMIC = b2_dynamicBody
};

class CompositeObject {
   public:
    // 傳入多個 Shape 作為子物件
    CompositeObject() = default;
    CompositeObject(
        std::vector<std::shared_ptr<Shape>> shapes,
        BodyType bodyType = BodyType::STATIC,
        glm::vec2 position = {0.0F, 0.0F},
        float rotation = 0.0F);

    ~CompositeObject() = default;

    virtual void Update();

    virtual void AttachToWorld(b2WorldId world);

   protected:
    Util::Renderer m_Renderer;
    std::vector<std::shared_ptr<Shape>> m_Shapes;
    BodyType m_BodyType;
    glm::vec2 m_Position;
    float m_Rotation;
    b2BodyId m_b2BodyId = b2_nullBodyId;
    // float m_Magnetism = 0;
};

}  // namespace GameWorld

#endif