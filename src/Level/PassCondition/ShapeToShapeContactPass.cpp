#include "Level/PassCondition/ShapeToShapeContactPass.hpp"

ShapeToShapeContactPass::ShapeToShapeContactPass(
    b2ShapeId shapeA,
    b2ShapeId shapeB,
    TriggerType triggerType,
    int duration)
    : PassCondition(triggerType, duration), m_ShapeA(shapeA), m_ShapeB(shapeB) {
    if (b2Shape_IsSensor(shapeA)) {
        b2Shape_EnableSensorEvents(shapeA, true);
    } else {
        b2Shape_EnableContactEvents(shapeA, true);
    }
    if (b2Shape_IsSensor(shapeB)) {
        b2Shape_EnableSensorEvents(shapeB, true);
    } else {
        b2Shape_EnableContactEvents(shapeB, true);
    }
}

void ShapeToShapeContactPass::OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) {
    if ((B2_ID_EQUALS(shapeA, m_ShapeA) && B2_ID_EQUALS(shapeB, m_ShapeB)) ||
        (B2_ID_EQUALS(shapeB, m_ShapeA) && B2_ID_EQUALS(shapeA, m_ShapeB))) {
        if (triggerType == m_TriggerType) {
            m_IsTriggered = true;
        } else {
            m_IsTriggered = false;
        }
    }
}
