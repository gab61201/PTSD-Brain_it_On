#ifndef GAMEWORLD_BASE_OBJECT_HPP
#define GAMEWORLD_BASE_OBJECT_HPP

#include <glm/vec2.hpp>
#include <memory>
#include <string>

#include "Util/GameObject.hpp"

class b2Body;

namespace GameWorld {

class Physics;

enum class Shape {
    RECTANGLE,
    CIRCLE,
    TRIANGLE
};

enum class BodyType {
    STATIC,
    DYNAMIC,
    KINEMATIC
};

class BaseObject {
   public:
    BaseObject(
        Physics& physics,
        Shape shape,
        glm::vec2 size,
        glm::vec2 position,
        float rotation,
        BodyType bodyType,
        bool isSensor = false);

    BaseObject() = default;

    // 將 Box2D 的狀態單向同步到 PTSD 視覺元件
    void Sync();

    std::shared_ptr<Util::GameObject> GetVisual() const;

    glm::vec2 GetPosition() const;
    void SetPosition(glm::vec2 posPixels);
    float GetRotation() const;
    void SetRotation(float angleRadians);

    // 這些主要是給 CompoundObject 使用，讓它能存取內部的 Body
    b2Body* GetBody() const { return m_Body; }
    Shape GetShape() const { return m_Shape; }
    glm::vec2 GetSize() const { return m_Size; }

   private:
    std::shared_ptr<Util::GameObject> m_Visual;
    b2Body* m_Body = nullptr;
    Shape m_Shape;
    glm::vec2 m_Size;
};

}  // namespace GameWorld

#endif  // GAMEWORLD_BASE_OBJECT_HPP
