#include "Level/PassCondition/PassCondition.hpp"

#include <box2d/box2d.h>

#include "Constants.hpp"

PassCondition::PassCondition(
    TriggerType triggerType,
    int duration)
    : m_TriggerType(triggerType),
      m_Duration(duration) {}

bool PassCondition::Check(b2ContactEvents events) {
    for (int i = 0; i < events.beginCount; i++) {
        const b2ContactBeginTouchEvent& event = events.beginEvents[i];
        OnContactEvent(event.shapeIdA, event.shapeIdB, TriggerType::TOUCHING);
    }
    for (int i = 0; i < events.endCount; i++) {
        const b2ContactEndTouchEvent& event = events.endEvents[i];
        OnContactEvent(event.shapeIdA, event.shapeIdB, TriggerType::SEPARATED);
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
