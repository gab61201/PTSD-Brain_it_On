#pragma once

#include "Level/PassCondition/PassCondition.hpp"

class ShapeToAnythingContactPass : public PassCondition {
   public:
    ShapeToAnythingContactPass(
        b2ShapeId shape,
        TriggerType triggerType,
        int duration);
    ~ShapeToAnythingContactPass() = default;

   private:
    void OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) override;
    b2ShapeId m_Shape = b2_nullShapeId;
};
