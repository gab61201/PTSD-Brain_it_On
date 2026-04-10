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
    (void)world;
    m_FixtureA = m_BaseObjectA->m_Fixture;
    if (m_BaseObjectB) {
        m_FixtureB = m_BaseObjectB->m_Fixture;
    }
}

void OneToOneContactPass::OnContactEvent(Physics::Shape fixtureA, Physics::Shape fixtureB, TriggerType triggerType) {
    if (B2_ID_EQUALS(fixtureA, m_FixtureA) || B2_ID_EQUALS(fixtureB, m_FixtureA)) {  // 找到 A 物體
        if ((B2_IS_NON_NULL(m_FixtureB) && (B2_ID_EQUALS(fixtureA, m_FixtureB) || B2_ID_EQUALS(fixtureB, m_FixtureB)))  // 有指定 B 且找到 B
            || B2_IS_NULL(m_FixtureB)) {                                                               // 無指定 B
            if (triggerType == m_TriggerType) {
                m_Timer++;
            } else {
                m_Timer = 0;
            }
        }
    }
}