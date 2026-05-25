#ifndef LEVEL_ONE_TO_ONE_CONTACT_PASS_HPP
#define LEVEL_ONE_TO_ONE_CONTACT_PASS_HPP

#include "Level/PassCondition/PassCondition.hpp"

class OneToOneContactPass : public PassCondition {
   public:
    OneToOneContactPass(
        b2ShapeId shapeA,
        b2ShapeId shapeB,
        TriggerType triggerType,
        int duration);
    OneToOneContactPass(
        b2ShapeId shape,
        TriggerType triggerType,
        int duration);
    ~OneToOneContactPass() = default;

   private:
    void OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) override;
    b2ShapeId m_ShapeA = b2_nullShapeId;
    b2ShapeId m_ShapeB = b2_nullShapeId;
};

#endif  // LEVEL_ONE_TO_ONE_CONTACT_PASS_HPP