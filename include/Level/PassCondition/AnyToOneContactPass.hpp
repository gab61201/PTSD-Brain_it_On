#pragma once

#include <vector>

#include "Level/PassCondition/PassCondition.hpp"

// 多對一接觸通關條件：m_ShapesA 中任意一個形狀與 m_ShapeB 發生指定接觸事件即通關
class AnyToOneContactPass : public PassCondition {
   public:
    AnyToOneContactPass(
        std::vector<b2ShapeId> shapesA,
        b2ShapeId shapeB,
        TriggerType triggerType,
        int duration);
    ~AnyToOneContactPass() = default;

   private:
    void OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) override;
    std::vector<b2ShapeId> m_ShapesA;
    b2ShapeId m_ShapeB = b2_nullShapeId;
};
