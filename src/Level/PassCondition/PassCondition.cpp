#include "Level/PassCondition/PassCondition.hpp"

#include <box2d/box2d.h>

#define FPS 60

PassCondition::PassCondition(
    TriggerType triggerType,
    int duration)
    : m_TriggerType(triggerType),
      m_Duration(duration) {}

bool PassCondition::Check(b2ContactEvents events) {
    if (m_Timer > m_Duration * FPS) {
        return true;
    }
    for (int i = 0; i < events.beginCount; i++) {
        const b2ContactBeginTouchEvent& event = events.beginEvents[i];
        OnContactEvent(event.shapeIdA, event.shapeIdB, TriggerType::TOUCHING);
    }
    for (int i = 0; i < events.endCount; i++) {
        const b2ContactEndTouchEvent& event = events.endEvents[i];
        OnContactEvent(event.shapeIdA, event.shapeIdB, TriggerType::SEPARATED);
    }
    if (m_Timer > 0) {
        m_Timer++;
    }
    return m_Timer > m_Duration * FPS;
}