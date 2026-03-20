#ifndef LEVEL_DATA_HPP
#define LEVEL_DATA_HPP

#include <map>

#include "GameWorld/PhysicalWorld.hpp"
#include "Level/PassCondition.hpp"

enum class LevelId {
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
    LEVEL_5
};

struct LevelData {
    float timeout = 30.0F;
    std::shared_ptr<GameWorld::PhysicalWorld> world;
};

LevelData GetLevelData(LevelId);

#endif
