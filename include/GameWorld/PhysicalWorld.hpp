#ifndef PHYSICAL_WORLD_HPP
#define PHYSICAL_WORLD_HPP

#include "GameWorld/CompositeObject.hpp"
#include "GameWorld/DrawnObject.hpp"
#include "Level/PassCondition/PassCondition.hpp"

namespace GameWorld {

class PhysicalWorld {
   public:
    PhysicalWorld(std::vector<std::shared_ptr<CompositeObject>> compositeObjects, PassCondition* passCondition);
    ~PhysicalWorld() = default;

    // 更新碰撞並渲染
    void Start();
    void Stop();
    void DrawObject(glm::vec2 position);
    void EndDrawing();
    bool IsPassed();
    void Update();

   private:
    b2World m_b2World;
    // 地圖原有的物件
    std::vector<std::shared_ptr<CompositeObject>> m_CompositeObject;
    // 玩家畫的物件
    std::vector<std::shared_ptr<DrawnObject>> m_DrawnObjects;
    std::shared_ptr<DrawnObject> m_LastDrawingObject;
    PassCondition* m_PassCondition;
    bool m_IsActive = false;
};

}  // namespace GameWorld

#endif  // WORLD_HPP
