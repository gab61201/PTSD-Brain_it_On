#ifndef PHYSICS_WORLD_HPP
#define PHYSICS_WORLD_HPP

#include <glm/vec2.hpp>
#include <memory>

class b2Body;

class PhysicsWorld {
   public:
    PhysicsWorld();
    ~PhysicsWorld();

    PhysicsWorld(const PhysicsWorld&) = delete;
    PhysicsWorld(PhysicsWorld&&) noexcept;
    PhysicsWorld& operator=(const PhysicsWorld&) = delete;
    PhysicsWorld& operator=(PhysicsWorld&&) noexcept;

    void InitializeScene();
    void Step();

    // 工廠方法：建立物理物件（接受像素座標）
    b2Body* CreateCircle(glm::vec2 posPixels, float radiusPixels, float rotationRadians, bool isDynamic);
    b2Body* CreateBox(glm::vec2 posPixels, glm::vec2 halfSizePixels, float rotationRadians, bool isDynamic);

    // Compound body 用：建立不帶 fixture 的 body，再動態掛 fixture
    b2Body* CreateEmptyBody(glm::vec2 posPixels, float rotationRadians, bool isDynamic);
    void AddCircleFixture(b2Body* body, glm::vec2 localOffsetPixels, float radiusPixels);
    void AddBoxFixture(b2Body* body, glm::vec2 localOffsetPixels, glm::vec2 halfSizePixels, float localRotation);

    [[nodiscard]] glm::vec2 GetCirclePositionPixels() const;
    [[nodiscard]] float GetCircleRotationRadians() const;

    void SetCirclePosition(glm::vec2 pixelPos);
    void ResetCircle();

   private:
    struct Impl;
    std::unique_ptr<Impl> m_Impl;
};

#endif  // PHYSICS_WORLD_HPP
