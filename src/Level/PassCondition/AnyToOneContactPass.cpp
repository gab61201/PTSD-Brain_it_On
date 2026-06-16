#include "Level/PassCondition/AnyToOneContactPass.hpp"

AnyToOneContactPass::AnyToOneContactPass(
    std::vector<b2ShapeId> shapesA,
    b2ShapeId shapeB,
    TriggerType triggerType,
    int duration)
    : PassCondition(triggerType, duration), m_ShapesA(std::move(shapesA)), m_ShapeB(shapeB) {
    for (auto& shape : m_ShapesA) {
        if (b2Shape_IsSensor(shape)) {
            b2Shape_EnableSensorEvents(shape, true);
        } else {
            b2Shape_EnableContactEvents(shape, true);
        }
    }
    if (b2Shape_IsSensor(shapeB)) {
        b2Shape_EnableSensorEvents(shapeB, true);
    } else {
        b2Shape_EnableContactEvents(shapeB, true);
    }
}

void AnyToOneContactPass::OnContactEvent(
    b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) {
    // 確認事件中有 m_ShapeB
    bool hasB = B2_ID_EQUALS(shapeA, m_ShapeB) || B2_ID_EQUALS(shapeB, m_ShapeB);
    if (!hasB) return;

    // 確認事件中有 m_ShapesA 中的任意一個
    bool hasAny = false;
    for (const auto& s : m_ShapesA) {
        if (B2_ID_EQUALS(shapeA, s) || B2_ID_EQUALS(shapeB, s)) {
            hasAny = true;
            break;
        }
    }
    if (!hasAny) return;

    if (triggerType == m_TriggerType) {
        m_IsTriggered = true;
    } else {
        m_IsTriggered = false;
    }
}
