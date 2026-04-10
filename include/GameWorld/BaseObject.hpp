#ifndef BASE_OBJECT_HPP
#define BASE_OBJECT_HPP

#include "Physics/Physics.hpp"

#include <glm/glm.hpp>
#include <memory>

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

namespace GameWorld {

enum class ShapeType {
    CIRCLE,
    RECTANGLE,
    TRIANGLE
};

class BaseObject {
   private:
    static Util::AssetStore<std::shared_ptr<Util::Image>> s_ImageCache;

   public:
    BaseObject(
        ShapeType shape,
        glm::vec2 size,
        glm::vec2 position,
        float rotation = 0.0F,
        bool isSensor = false);

    ~BaseObject() = default;

    void Update(glm::vec2 ParentObjectPosition, float ParentObjectRotation);

    void AttachToBody(Physics::BodyPtr body);

    std::shared_ptr<Util::GameObject> m_Visual;

    Physics::ShapePtr m_Fixture;

   protected:

    ShapeType m_ShapeType;
    glm::vec2 m_Size;
    glm::vec2 m_RelativePosition;
    float m_RelativeRotation;
    bool m_IsSensor;
};

}  // namespace GameWorld

#endif  // BASE_OBJECT_HPP
