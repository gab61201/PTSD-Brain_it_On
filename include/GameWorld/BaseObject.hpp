#ifndef BASE_OBJECT_HPP
#define BASE_OBJECT_HPP

#include <box2d/b2_body.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

#include <glm/glm.hpp>
#include <memory>

#include "Util/GameObject.hpp"

namespace GameWorld {

enum class ShapeType {
    CIRCLE,
    RECTANGLE,
    TRIANGLE
};

class BaseObject {
   public:
    BaseObject(
        ShapeType shape,
        glm::vec2 size,
        glm::vec2 position,
        float rotation = 0.0F,
        bool isSensor = false);

    ~BaseObject() = default;

    void Update(glm::vec2 ParentObjectPosition, float ParentObjectRotation);

    void AttachToBody(b2Body* body);

    std::shared_ptr<Util::GameObject> m_Visual;

    b2Fixture* m_Fixture;

   protected:

    ShapeType m_ShapeType;
    glm::vec2 m_Size;
    glm::vec2 m_RelativePosition;
    float m_RelativeRotation;
    bool m_IsSensor;
};

}  // namespace GameWorld

#endif  // BASE_OBJECT_HPP
