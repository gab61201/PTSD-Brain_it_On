#pragma once

#include "Level/PassCondition/PassCondition.hpp"

class ShapeToShapeContactPass : public PassCondition {
   public:
    ShapeToShapeContactPass(
        b2ShapeId shapeA,
        b2ShapeId shapeB,
        TriggerType triggerType,
        int duration);
    ~ShapeToShapeContactPass() = default;

   private:
    void OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) override;
    b2ShapeId m_ShapeA = b2_nullShapeId;
    b2ShapeId m_ShapeB = b2_nullShapeId;
};
