#pragma once

#include <functional>
#include <map>
#include <string>

#include "GameWorld/PhysicalWorld.hpp"
#include "Level/PassCondition/PassCondition.hpp"

enum class LevelId {
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
    LEVEL_5
};

struct LevelConfig {
    float timeout = 30.0F;
    int strokeLimit = 3;
    std::string targetText = "";
    std::shared_ptr<GameWorld::PhysicalWorld> world;
    std::shared_ptr<PassCondition> passCondition;
};

struct LevelResult {
    LevelId levelId = LevelId::LEVEL_1;
    bool passed = false;
    float goalTime = 0.0f;
    float solvedTime = 0.0f;
    int goalStroke = 0;
    int usedStroke = 0;
    std::string screenshotFilename;
};


using LevelFunction = std::function<LevelConfig()>;

// 取得全域 map（用 function 包一層避免 static 初始化順序問題）
std::map<LevelId, LevelFunction>& GetLevelRegistry();

// 對外 API
LevelConfig GetLevelConfig(LevelId id);

// 註冊用 helper
void RegisterLevel(LevelId id, LevelFunction function);
