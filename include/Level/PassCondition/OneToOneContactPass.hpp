#ifndef ONE_TO_ONE_CONTACT_PASS_HPP
#define ONE_TO_ONE_CONTACT_PASS_HPP

#include "Level/PassCondition/PassCondition.hpp"

class OneToOneContactPass : public PassCondition {
   public:
    OneToOneContactPass(
        std::shared_ptr<GameWorld::BaseObject> baseObjectA,
        std::shared_ptr<GameWorld::BaseObject> baseObjectB,
        TriggerType triggerType,
        int duration
    );
    OneToOneContactPass(
        std::shared_ptr<GameWorld::BaseObject> baseObject,
        TriggerType triggerType,
        int duration
    );
    ~OneToOneContactPass() = default;
    void AttachToWorld(b2World* world) override;

   private:
    void OnContactEvent(b2Fixture* fixtureA, b2Fixture* fixtureB, TriggerType triggerType) override;
    std::shared_ptr<GameWorld::BaseObject> m_BaseObjectA = nullptr;
    std::shared_ptr<GameWorld::BaseObject> m_BaseObjectB = nullptr;
    b2Fixture* m_FixtureA = nullptr;
    b2Fixture* m_FixtureB = nullptr;
};

#endif  // ONE_TO_ONE_CONTACT_PASS_HPP