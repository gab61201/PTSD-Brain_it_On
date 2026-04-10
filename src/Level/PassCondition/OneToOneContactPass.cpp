#include "Level/PassCondition/OneToOneContactPass.hpp"

OneToOneContactPass::OneToOneContactPass(
    std::shared_ptr<GameWorld::BaseObject> baseObjectA,
    std::shared_ptr<GameWorld::BaseObject> baseObjectB,
    TriggerType triggerType,
    int duration)
    : PassCondition(triggerType, duration),
      m_BaseObjectA(baseObjectA),
      m_BaseObjectB(baseObjectB) {}

OneToOneContactPass::OneToOneContactPass(
    std::shared_ptr<GameWorld::BaseObject> baseObject,
    TriggerType triggerType,
    int duration)
    : PassCondition(triggerType, duration),
      m_BaseObjectA(baseObject) {}

void OneToOneContactPass::AttachToWorld(Physics::WorldPtr world) {
    m_FixtureA = m_BaseObjectA->m_Fixture;
    if (m_BaseObjectB) {
        m_FixtureB = m_BaseObjectB->m_Fixture;
    }
    world->SetContactListener(this);
}

void OneToOneContactPass::OnContactEvent(Physics::ShapePtr fixtureA, Physics::ShapePtr fixtureB, TriggerType triggerType) {
    if (fixtureA == m_FixtureA || fixtureB == m_FixtureA) {                     // 找到 A 物體
        if ((m_FixtureB && (fixtureA == m_FixtureB || fixtureB == m_FixtureB))  // 有指定 B 且找到 B
            || m_FixtureB == nullptr) {                                         // 無指定 B
            if (triggerType == m_TriggerType) {
                m_Timer++;
            } else {
                m_Timer = 0;
            }
        }
    }
}