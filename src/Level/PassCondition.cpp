#include "Level/PassCondition.hpp"

#include <box2d/b2_contact.h>

#define FPS 60

PassCondition::PassCondition(b2Fixture* fixtureA, b2Fixture* fixtureB, int time, Condition condition)
    : m_Fixture_A(fixtureA), m_Fixture_B(fixtureB), m_Time(time * FPS), m_Condition(condition) {}

bool PassCondition::Check() const {
    return m_IsPassed;
}

void PassCondition::BeginContact(b2Contact* contact) {
    if (m_IsPassed) {
        return;
    }

    if ((m_Fixture_A == contact->GetFixtureA() && m_Fixture_B == contact->GetFixtureB()) ||
        (m_Fixture_A == contact->GetFixtureB() && m_Fixture_B == contact->GetFixtureA())) {
        if (m_Condition == Condition::TOUCHING) {
            m_Counter++;
        } else if (m_Condition == Condition::SEPERATED) {
            m_Counter = 0;
        }
        
        if (m_Counter >= m_Time) {
            m_IsPassed = true;
        }
    }
}

void PassCondition::EndContact(b2Contact * contact) {
    if (m_IsPassed) {
        return;
    }

    if ((m_Fixture_A == contact->GetFixtureA() && m_Fixture_B == contact->GetFixtureB()) ||
        (m_Fixture_A == contact->GetFixtureB() && m_Fixture_B == contact->GetFixtureA())) {
        if (m_Condition == Condition::TOUCHING) {
            m_Counter = 0;
        } else if (m_Condition == Condition::SEPERATED) {
            m_Counter++;
        }

        if (m_Counter >= m_Time) {
            m_IsPassed = true;
        }
    }
}