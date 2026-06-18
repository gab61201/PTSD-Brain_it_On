#include "Level/PassCondition/GroupToShapeContactPass.hpp"

GroupToShapeContactPass::GroupToShapeContactPass(
    std::vector<b2ShapeId> shapeGroup,
    b2ShapeId targetShape,
    TriggerType triggerType,
    int duration)
    : PassCondition(triggerType, duration), m_ShapeGroup(std::move(shapeGroup)), m_TargetShape(targetShape) {
    for (auto& shape : m_ShapeGroup) {
        if (b2Shape_IsSensor(shape)) {
            b2Shape_EnableSensorEvents(shape, true);
        } else {
            b2Shape_EnableContactEvents(shape, true);
        }
    }
    if (b2Shape_IsSensor(targetShape)) {
        b2Shape_EnableSensorEvents(targetShape, true);
    } else {
        b2Shape_EnableContactEvents(targetShape, true);
    }
}

void GroupToShapeContactPass::OnContactEvent(
    b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) {
    bool hasTarget = B2_ID_EQUALS(shapeA, m_TargetShape) || B2_ID_EQUALS(shapeB, m_TargetShape);
    if (!hasTarget) return;

    bool hasAnyInGroup = false;
    for (const auto& s : m_ShapeGroup) {
        if (B2_ID_EQUALS(shapeA, s) || B2_ID_EQUALS(shapeB, s)) {
            hasAnyInGroup = true;
            break;
        }
    }
    if (!hasAnyInGroup) return;

    if (triggerType == m_TriggerType) {
        m_IsTriggered = true;
    } else {
        m_IsTriggered = false;
    }
}
