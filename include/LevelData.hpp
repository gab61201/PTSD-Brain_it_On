#ifndef LEVEL_DATA_HPP
#define LEVEL_DATA_HPP

#include <map>
#include "Physics/PhysicalObject.hpp"
#include "PassCondition.hpp"

enum class LevelId {
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
    LEVEL_5
};

struct LevelData {
    float timeout;
    std::vector<std::shared_ptr<PhysicalObject>> objects;
    std::vector<std::shared_ptr<Util::GameObject>> banned_areas;
    std::vector<PassCondition> pass_conditions;
    float pass_condition_check_duration;  // 通關檢測持續多久才算過關
};

LevelData GetLevelData(LevelId);

#endif
