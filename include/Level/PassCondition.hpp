#ifndef PASS_CONDITION_HPP
#define PASS_CONDITION_HPP

#include <box2d/b2_fixture.h>
#include <box2d/b2_world_callbacks.h>

enum class Condition {
    TOUCHING,
    SEPERATED
};

class PassCondition : public b2ContactListener {
   public:
    PassCondition(b2Fixture* fixtureA, b2Fixture* fixtureB, int time, Condition condition);
    ~PassCondition() = default;

    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;

    bool Check() const;

   private:
    b2Fixture* m_Fixture_A;
    b2Fixture* m_Fixture_B;
    int m_Time;
    Condition m_Condition;

    bool m_IsPassed = false;
    int m_Counter = 0;
};

#endif