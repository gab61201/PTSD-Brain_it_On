#include "Level/PassCondition/PassCondition.hpp"

#include <box2d/box2d.h>

#include "Constants.hpp"

PassCondition::PassCondition(
    TriggerType triggerType,
    int duration)
    : m_TriggerType(triggerType),
      m_Duration(duration) {}

bool PassCondition::Check(b2ContactEvents contactEvents, b2SensorEvents sensorEvents) {
    for (int i = 0; i < contactEvents.beginCount; i++) {
        const b2ContactBeginTouchEvent& event = contactEvents.beginEvents[i];
        OnContactEvent(event.shapeIdA, event.shapeIdB, TriggerType::TOUCHING);
    }
    for (int i = 0; i < contactEvents.endCount; i++) {
        const b2ContactEndTouchEvent& event = contactEvents.endEvents[i];
        OnContactEvent(event.shapeIdA, event.shapeIdB, TriggerType::SEPARATED);
    }
    for (int i = 0; i < sensorEvents.beginCount; i++) {
        const b2SensorBeginTouchEvent& event = sensorEvents.beginEvents[i];
        OnContactEvent(event.visitorShapeId, event.sensorShapeId, TriggerType::TOUCHING);
    }
    for (int i = 0; i < sensorEvents.endCount; i++) {
        const b2SensorEndTouchEvent& event = sensorEvents.endEvents[i];
        OnContactEvent(event.visitorShapeId, event.sensorShapeId, TriggerType::SEPARATED);
    }

    if (m_IsTriggered) {
        m_Timer++;
    } else {
        m_Timer = 0;
    }

    return m_IsTriggered && m_Timer >= m_Duration * FPS;
}

int PassCondition::GetContactCountDown() const {
    if (m_IsTriggered) {
        return std::max(0, m_Duration - m_Timer / FPS);
    }
    return 0;
}
