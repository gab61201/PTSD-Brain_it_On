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
    void AttachToWorld(Physics::WorldPtr world) override;

   private:
    void OnContactEvent(Physics::ShapePtr fixtureA, Physics::ShapePtr fixtureB, TriggerType triggerType) override;
    std::shared_ptr<GameWorld::BaseObject> m_BaseObjectA = nullptr;
    std::shared_ptr<GameWorld::BaseObject> m_BaseObjectB = nullptr;
    Physics::ShapePtr m_FixtureA = nullptr;
    Physics::ShapePtr m_FixtureB = nullptr;
};

#endif  // ONE_TO_ONE_CONTACT_PASS_HPP