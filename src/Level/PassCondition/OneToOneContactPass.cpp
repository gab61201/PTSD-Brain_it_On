#include "Level/PassCondition/OneToOneContactPass.hpp"

OneToOneContactPass::OneToOneContactPass(
    b2ShapeId shapeA,
    b2ShapeId shapeB,
    TriggerType triggerType,
    int duration)
    : PassCondition(triggerType, duration), m_ShapeA(shapeA), m_ShapeB(shapeB) {
        if (b2Shape_IsSensor(shapeA)){
            b2Shape_EnableSensorEvents(shapeA, true);
        }else{
            b2Shape_EnableContactEvents(shapeA, true);
        }
        if (b2Shape_IsSensor(shapeB)){
            b2Shape_EnableSensorEvents(shapeB, true);
        }else{
            b2Shape_EnableContactEvents(shapeB, true);
        }
    }

OneToOneContactPass::OneToOneContactPass(
    b2ShapeId shape,
    TriggerType triggerType,
    int duration)
    : PassCondition(triggerType, duration), m_ShapeA(shape), m_ShapeB(b2_nullShapeId) {}

void OneToOneContactPass::OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) {
    if (B2_ID_EQUALS(shapeA, m_ShapeA) || B2_ID_EQUALS(shapeB, m_ShapeA)) {  // 找到 A 物體
        if ((B2_IS_NON_NULL(m_ShapeB) && (B2_ID_EQUALS(shapeA, m_ShapeB) || B2_ID_EQUALS(shapeB, m_ShapeB)))  // 有指定 B 且找到 B
            || B2_IS_NULL(m_ShapeB)) {                                                               // 無指定 B
            if (triggerType == m_TriggerType) {
                m_Timer++;
            } else {
                m_Timer = 0;
            }
        }
    }
}