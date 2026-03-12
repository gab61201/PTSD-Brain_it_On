#ifndef PHYSICS_WORLD_HPP
#define PHYSICS_WORLD_HPP

#include <memory>

#include "pch.hpp"  // IWYU pragma: export

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

    [[nodiscard]] glm::vec2 GetCirclePositionPixels() const;
    [[nodiscard]] float GetCircleRotationRadians() const;

    void SetCirclePosition(glm::vec2 pixelPos);
    void ResetCircle();

   private:
    struct Impl;
    std::unique_ptr<Impl> m_Impl;
};

#endif  // PHYSICS_WORLD_HPP
