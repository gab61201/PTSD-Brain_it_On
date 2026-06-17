#include "Level/PassCondition/ShapeToAnythingContactPass.hpp"

ShapeToAnythingContactPass::ShapeToAnythingContactPass(
    b2ShapeId shape,
    TriggerType triggerType,
    int duration)
    : PassCondition(triggerType, duration), m_Shape(shape) {
    // 統一開啟事件，避免依賴另一方
    if (b2Shape_IsSensor(shape)) {
        b2Shape_EnableSensorEvents(shape, true);
    } else {
        b2Shape_EnableContactEvents(shape, true);
    }
}

void ShapeToAnythingContactPass::OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) {
    if (B2_ID_EQUALS(shapeA, m_Shape) || B2_ID_EQUALS(shapeB, m_Shape)) {
        if (triggerType == m_TriggerType) {
            m_IsTriggered = true;
        } else {
            m_IsTriggered = false;
        }
    }
}
