#pragma once

#include <box2d/box2d.h>


#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/CompositeObject/CompositeObject.hpp"
#include "GameWorld/CompositeObject/DrawnObject.hpp"
#include "GameWorld/DrawingIndicator.hpp"

namespace GameWorld {

class PhysicalWorld {
   public:
    PhysicalWorld(
        std::vector<std::shared_ptr<CompositeObject>> compositeObjects,
        std::shared_ptr<Boundary> boundary = nullptr);
    ~PhysicalWorld();

    // 更新碰撞並渲染
    void Start() { m_IsActive = true; }
    void Stop() { m_IsActive = false; }
    void DrawNewObject(glm::vec2 position);
    void DrawingObject(glm::vec2 position);
    void EndDrawing();
    void Update();
    int GetDrawnObjectCount() const { return m_DrawnObjectCount; }
    b2ContactEvents GetContactEvents() { return b2World_GetContactEvents(m_b2WorldId); }
    b2SensorEvents GetSensorEvents() { return b2World_GetSensorEvents(m_b2WorldId); }
   private:
    b2WorldId m_b2WorldId;
    // 地圖原有的物件
    std::vector<std::shared_ptr<CompositeObject>> m_CompositeObject;
    std::shared_ptr<Boundary> m_Boundary;
    // 玩家畫的物件
    int m_DrawnObjectCount = 0;
    std::shared_ptr<DrawnObject> m_LastDrawingObject;
    DrawingIndicator m_DrawingIndicator;
    bool m_IsActive = false;
    std::vector<b2ShapeId> m_ForbiddenShapeIds;
};

}  // namespace GameWorld
