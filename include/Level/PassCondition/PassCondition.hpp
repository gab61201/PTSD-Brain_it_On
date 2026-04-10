#ifndef PASS_CONDITION_HPP
#define PASS_CONDITION_HPP

#include "Physics/Physics.hpp"

#include "GameWorld/BaseObject.hpp"

// 放一個物體到盒子內
// 將球放到盒子內

// 將球放到任意盒子內
// 分類球體

enum class TriggerType {
    TOUCHING,
    SEPARATED
};

class PassCondition {
   public:
    PassCondition() = default;
    PassCondition(
        TriggerType triggerType,
        int duration);
    virtual ~PassCondition() = default;

    virtual void AttachToWorld(Physics::WorldPtr world) = 0;
    void ConsumeContactEvents(Physics::World world);
    void Update();
    bool Check() const;

   protected:
    virtual void OnContactEvent(Physics::Shape fixtureA, Physics::Shape fixtureB, TriggerType triggerType) = 0;
    TriggerType m_TriggerType;
    int m_Duration;

    bool m_IsPassed = false;
    int m_Timer = 0;
};

#endif