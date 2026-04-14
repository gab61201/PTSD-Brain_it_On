#ifndef PHYSICAL_WORLD_HPP
#define PHYSICAL_WORLD_HPP

#include <box2d/box2d.h>

#include "GameWorld/CompositeObject.hpp"
#include "GameWorld/DrawnObject.hpp"
#include "GameWorld/MagnetObject.hpp"

namespace GameWorld {

class PhysicalWorld {
   public:
    PhysicalWorld(std::vector<std::shared_ptr<CompositeObject>> compositeObjects);
    ~PhysicalWorld();

    // 更新碰撞並渲染
    void Start();
    void Stop();
    void DrawObject(glm::vec2 position);
    void EndDrawing();
    void Update();
    b2ContactEvents GetContactEvents();

   private:
    b2WorldId m_b2WorldId;
    // 地圖原有的物件
    std::vector<std::shared_ptr<CompositeObject>> m_CompositeObject;
    // 玩家畫的物件
    std::vector<std::shared_ptr<DrawnObject>> m_DrawnObjects;
    std::shared_ptr<DrawnObject> m_LastDrawingObject;
    bool m_IsActive = false;
};

}  // namespace GameWorld

#endif  // WORLD_HPP
