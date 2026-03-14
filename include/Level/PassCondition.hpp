#ifndef PASS_CONDITION_HPP
#define PASS_CONDITION_HPP

#include "Physics/PhysicalObject.hpp"
#include "Util/GameObject.hpp"

enum class Condition {
    TOUCHING,
    SEPERATED,
    OVERLAP
};

class PassCondition {
   public:
    PassCondition(Util::GameObject, PhysicalObject, Condition);
    PassCondition(PhysicalObject, PhysicalObject, Condition);
    bool Check();
};

#endif