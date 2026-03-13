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

    // 將交由子類別去實作對應的建構/初始化邏輯
    // std::shared_ptr<Util::GameObject> GetVisual() const;
    // ...

protected:
    PhysicalObject() = default; // 確保不能直接實例化

public:
    // 拿到 PTSD 的 GameObject
    std::shared_ptr<Util::GameObject> GetVisual() const;

    // 單向同步 Box2D -> PTSD
    virtual void Sync();

    // --- setter / getter 是 debug 用，實務上應該不需要 ---

    void SetPosition(glm::vec2 posPixels);
    glm::vec2 GetPosition() const;

    void SetRotation(float angleRadians);
    float GetRotation() const;

protected:
    b2Body* m_Body = nullptr;                    // Box2D
    std::shared_ptr<Util::GameObject> m_Visual;  // PTSD 畫面
};

#endif
