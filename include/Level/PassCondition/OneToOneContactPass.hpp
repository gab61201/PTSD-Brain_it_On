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
    void AttachToWorld(b2WorldId world) override;

   private:
    void OnContactEvent(b2ShapeId fixtureA, b2ShapeId fixtureB, TriggerType triggerType) override;
    std::shared_ptr<GameWorld::BaseObject> m_BaseObjectA = nullptr;
    std::shared_ptr<GameWorld::BaseObject> m_BaseObjectB = nullptr;
    b2ShapeId m_FixtureA = b2_nullShapeId;
    b2ShapeId m_FixtureB = b2_nullShapeId;
};

#endif  // ONE_TO_ONE_CONTACT_PASS_HPP