#pragma once

#include <vector>

#include "Level/PassCondition/PassCondition.hpp"

class GroupToShapeContactPass : public PassCondition {
   public:
    GroupToShapeContactPass(
        std::vector<b2ShapeId> shapeGroup,
        b2ShapeId targetShape,
        TriggerType triggerType,
        int duration);
    ~GroupToShapeContactPass() = default;

   private:
    void OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) override;
    std::vector<b2ShapeId> m_ShapeGroup;
    b2ShapeId m_TargetShape = b2_nullShapeId;
};
