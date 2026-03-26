#ifndef LEVEL_DATA_HPP
#define LEVEL_DATA_HPP

#include <functional>

#include "GameWorld/PhysicalWorld.hpp"
#include "Level/PassCondition/PassCondition.hpp"

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

using LevelFunction = std::function<LevelData()>;

// 取得全域 map（用 function 包一層避免 static 初始化順序問題）
std::unordered_map<LevelId, LevelFunction>& GetLevelRegistry();

// 對外 API
LevelData GetLevelData(LevelId id);

// 註冊用 helper
void RegisterLevel(LevelId id, LevelFunction function);

#endif
