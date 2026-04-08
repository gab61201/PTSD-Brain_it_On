#include "Level/PassCondition/PassCondition.hpp"

#define FPS 60

PassCondition::PassCondition(
    TriggerType triggerType,
    int duration)
    : m_TriggerType(triggerType),
      m_Duration(duration) {}

void PassCondition::BeginContact(b2Contact* contact) {
    OnContactEvent(contact->GetFixtureA(), contact->GetFixtureB(), TriggerType::TOUCHING);
}

void PassCondition::EndContact(b2Contact* contact) {
    OnContactEvent(contact->GetFixtureA(), contact->GetFixtureB(), TriggerType::SEPARATED);
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