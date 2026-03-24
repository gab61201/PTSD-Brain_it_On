#ifndef PASS_CONDITION_HPP
#define PASS_CONDITION_HPP

#include <box2d/b2_world_callbacks.h>

#include "GameWorld/BaseObject.hpp"

enum class Condition {
    TOUCHING,
    SEPERATED
};

class PassCondition : public b2ContactListener {
   public:
    PassCondition(
        std::shared_ptr<GameWorld::BaseObject> objectA,
        std::shared_ptr<GameWorld::BaseObject> objectB,
        Condition condition,
        int time
    );
    ~PassCondition() = default;

    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
    void Update();
    bool Check() const;

   private:
    std::shared_ptr<GameWorld::BaseObject> m_ObjectA;
    std::shared_ptr<GameWorld::BaseObject> m_ObjectB;
    Condition m_Condition;
    int m_Time;

    bool m_IsPassed = false;
    int m_Counter = 0;
};

#endif