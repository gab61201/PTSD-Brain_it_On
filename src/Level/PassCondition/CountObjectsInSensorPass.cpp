#include "Level/PassCondition/CountObjectsInSensorPass.hpp"

#include <algorithm>

CountObjectsInSensorPass::CountObjectsInSensorPass(
    std::vector<b2ShapeId> objects,
    b2ShapeId sensor,
    int requiredCount,
    int duration,
    CountComparison comparison)
    : PassCondition(TriggerType::TOUCHING, duration),
      m_Objects(std::move(objects)),
      m_Sensor(sensor),
      m_RequiredCount(requiredCount),
      m_Comparison(comparison) {
    for (auto& obj : m_Objects) {
        b2Shape_EnableSensorEvents(obj, true);
    }
    b2Shape_EnableSensorEvents(m_Sensor, true);
}

void CountObjectsInSensorPass::OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) {
    bool isSensor = B2_ID_EQUALS(shapeA, m_Sensor) || B2_ID_EQUALS(shapeB, m_Sensor);
    if (!isSensor) return;

    b2ShapeId visitor = B2_ID_EQUALS(shapeA, m_Sensor) ? shapeB : shapeA;

    bool isTracked = false;
    for (const auto& obj : m_Objects) {
        if (B2_ID_EQUALS(obj, visitor)) {
            isTracked = true;
            break;
        }
    }
    if (!isTracked) return;

    if (triggerType == TriggerType::TOUCHING) {
        bool alreadyIn = false;
        for (const auto& in : m_ObjectsInside) {
            if (B2_ID_EQUALS(in, visitor)) {
                alreadyIn = true;
                break;
            }
        }
        if (!alreadyIn) {
            m_ObjectsInside.push_back(visitor);
        }
    } else {
        auto it = std::find_if(m_ObjectsInside.begin(), m_ObjectsInside.end(),
                               [&](const b2ShapeId& id) { return B2_ID_EQUALS(id, visitor); });
        if (it != m_ObjectsInside.end()) {
            m_ObjectsInside.erase(it);
        }
    }

    m_IsTriggered = EvaluateCount();
}

bool CountObjectsInSensorPass::EvaluateCount() const {
    int count = static_cast<int>(m_ObjectsInside.size());
    switch (m_Comparison) {
        case CountComparison::EXACTLY:
            return count == m_RequiredCount;
        case CountComparison::AT_LEAST:
            return count >= m_RequiredCount;
        case CountComparison::AT_MOST:
            return count <= m_RequiredCount;
    }
    return false;
}
