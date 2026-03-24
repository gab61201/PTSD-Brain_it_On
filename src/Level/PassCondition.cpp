#include "Level/PassCondition.hpp"

#include <box2d/b2_contact.h>

#define FPS 60

PassCondition::PassCondition(
    std::shared_ptr<GameWorld::BaseObject> objectA,
    std::shared_ptr<GameWorld::BaseObject> objectB,
    Condition condition,
    int time
) : m_ObjectA(objectA), m_ObjectB(objectB), m_Condition(condition), m_Time(time) {}

void PassCondition::BeginContact(b2Contact* contact) {
    if (m_IsPassed) {
        return;
    }
    
    if ((m_ObjectA->m_Fixture == contact->GetFixtureA() && m_ObjectB->m_Fixture == contact->GetFixtureB()) ||
    (m_ObjectA->m_Fixture == contact->GetFixtureB() && m_ObjectB->m_Fixture == contact->GetFixtureA())) {
        if (m_Condition == Condition::TOUCHING) {
            m_Counter = 1;
        } else if (m_Condition == Condition::SEPERATED) {
            m_Counter = 0;
        }
    }
}

void PassCondition::EndContact(b2Contact * contact) {
    if (m_IsPassed) {
        return;
    }
    
    if ((m_ObjectA->m_Fixture == contact->GetFixtureA() && m_ObjectB->m_Fixture == contact->GetFixtureB()) ||
    (m_ObjectA->m_Fixture == contact->GetFixtureB() && m_ObjectB->m_Fixture == contact->GetFixtureA())) {
        if (m_Condition == Condition::TOUCHING) {
            m_Counter = 0;
        } else if (m_Condition == Condition::SEPERATED) {
            m_Counter = 1;
        }
    }
}

void PassCondition::Update() {
    if (m_Counter > 0) {
        m_Counter++;
    }
    if (m_Counter >= m_Time * FPS) {
        m_IsPassed = true;
        m_Counter = 0;
    }
    
}

bool PassCondition::Check() const {
    return m_IsPassed;
}