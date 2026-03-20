#ifndef PHYSICAL_WORLD_HPP
#define PHYSICAL_WORLD_HPP

#include "GameWorld/CompositeObject.hpp"
#include "GameWorld/DrawnObject.hpp"

namespace GameWorld {

class PhysicalWorld {
   public:
    PhysicalWorld(std::vector<std::shared_ptr<CompositeObject>> compositeObjects);
    ~PhysicalWorld() = default;

    // 更新碰撞並渲染
    void Start();
    void DrawObject(glm::vec2 position);
    void EndDrawing();
    void Update();

   private:
    b2World m_b2World;
    std::vector<std::shared_ptr<CompositeObject>> m_CompositeObject;
    std::vector<std::shared_ptr<DrawnObject>> m_DrawnObjects;

    std::shared_ptr<DrawnObject> m_DrawingObject;
    bool m_IsActive = false;
};

}  // namespace GameWorld

#endif  // WORLD_HPP
