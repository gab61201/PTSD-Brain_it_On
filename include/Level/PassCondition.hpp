#ifndef PASS_CONDITION_HPP
#define PASS_CONDITION_HPP

#include "GameWorld/PhysicalWorld.hpp"
#include "Util/GameObject.hpp"

enum class Condition {
    TOUCHING,
    SEPERATED,
    OVERLAP
};

class PassCondition {
   public:
    PassCondition();
    bool Check();
};

#endif