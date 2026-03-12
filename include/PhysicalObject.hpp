#ifndef PHYSICAL_OBJECT_HPP
#define PHYSICAL_OBJECT_HPP

#include <glm/vec2.hpp>
#include <memory>

#include "Util/GameObject.hpp"

// 前置宣告（就可以不用 include）
class b2Body;
class PhysicsWorld;

class PhysicalObject {
   public:
    virtual ~PhysicalObject() = default;

    static std::shared_ptr<PhysicalObject> CreateCircle(
        PhysicsWorld& world,    // 物理世界
        glm::vec2 posPixels,    // PTSD 像素座標
        glm::vec2 sizePixels,   // 長寬像素
        float rotationRadians,  // 旋轉（弧度）
        bool isDynamic          // 會不會動
    );

    static std::shared_ptr<PhysicalObject> CreateRectangle(
        PhysicsWorld& world,    // 物理世界
        glm::vec2 posPixels,    // PTSD 像素座標
        glm::vec2 sizePixels,   // 長寬像素
        float rotationRadians,  // 旋轉（弧度）
        bool isDynamic          // 會不會動
    );

    // 拿到 PTSD 的 GameObject
    std::shared_ptr<Util::GameObject> GetVisual() const;

    void Sync();

    void SetPosition(glm::vec2 posPixels);
    glm::vec2 GetPosition() const;

   protected:
    b2Body* m_Body = nullptr;                    // Box2D
    std::shared_ptr<Util::GameObject> m_Visual;  // PTSD 畫面
};

#endif
