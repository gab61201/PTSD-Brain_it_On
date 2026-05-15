#include "Level/PassCondition/PassCondition.hpp"

#include <box2d/box2d.h>

#define FPS 60

PassCondition::PassCondition(
    TriggerType triggerType,
    int duration)
    : m_TriggerType(triggerType),
      m_Duration(duration) {}

bool PassCondition::Check(b2ContactEvents events) {
    // 通關
    if (m_Timer > m_Duration * FPS) {
        return true;
    // 未通關，但正在計時中
    } else if (m_Timer > 0) {
        m_Timer++;
    }
    for (int i = 0; i < events.beginCount; i++) {
        const b2ContactBeginTouchEvent& event = events.beginEvents[i];
        OnContactEvent(event.shapeIdA, event.shapeIdB, TriggerType::TOUCHING);
    }
    for (int i = 0; i < events.endCount; i++) {
        const b2ContactEndTouchEvent& event = events.endEvents[i];
        OnContactEvent(event.shapeIdA, event.shapeIdB, TriggerType::SEPARATED);
    }
    return m_Timer > m_Duration * FPS;
}


int PassCondition::GetContactCountDown() const {
    if (m_Timer > 0) {
        return m_Duration - m_Timer / FPS;
    }
    return 0;
}
