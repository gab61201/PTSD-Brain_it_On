#include "Level/PassCondition/PassCondition.hpp"

#define FPS 60

PassCondition::PassCondition(
    TriggerType triggerType,
    int duration)
    : m_TriggerType(triggerType),
      m_Duration(duration) {}

void PassCondition::ConsumeContactEvents(Physics::World world) {
    Physics::ContactEvents events = b2World_GetContactEvents(world);

    for (int i = 0; i < events.beginCount; i++) {
        const b2ContactBeginTouchEvent& event = events.beginEvents[i];
        OnContactEvent(event.shapeIdA, event.shapeIdB, TriggerType::TOUCHING);
    }

    for (int i = 0; i < events.endCount; i++) {
        const b2ContactEndTouchEvent& event = events.endEvents[i];
        OnContactEvent(event.shapeIdA, event.shapeIdB, TriggerType::SEPARATED);
    }
}

void PassCondition::Update() {
    if (m_IsPassed) {
        return;
    }
    if (m_Timer > 0) {
        m_Timer++;
    }
    if (m_Timer > m_Duration * FPS) {
        m_IsPassed = true;
        m_Timer = 0;
    }
}

bool PassCondition::Check() const {
    return m_IsPassed;
}