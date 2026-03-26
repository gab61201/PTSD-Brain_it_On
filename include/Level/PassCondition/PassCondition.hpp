#ifndef PASS_CONDITION_HPP
#define PASS_CONDITION_HPP

#include <box2d/box2d.h>

#include "GameWorld/BaseObject.hpp"

// 放一個物體到盒子內
// 將球放到盒子內

// 將球放到任意盒子內
// 分類球體

enum class TriggerType {
    TOUCHING,
    SEPERATED
};

class PassCondition : public b2ContactListener {
   public:
    PassCondition() = default;
    PassCondition(
        TriggerType triggerType,
        int duration);
    virtual ~PassCondition() = default;

    virtual void AttachToWorld(b2World* world) = 0;
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
    void Update();
    bool Check() const;

   protected:
    virtual void OnContactEvent(b2Fixture* fixtureA, b2Fixture* fixtureB, TriggerType triggerType) = 0;
    TriggerType m_TriggerType;
    int m_Duration;

    bool m_IsPassed = false;
    int m_Timer = 0;
};

#endif