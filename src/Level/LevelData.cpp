#include "Level/LevelData.hpp"
#include "Util/Text.hpp"

// TESTING
static LevelData LevelData_1() {
    LevelData levelData;
    levelData.timeout = 60.0F;
    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "LEVEL_1"));
    levelData.banned_areas.push_back(title);
    levelData.pass_condition_check_duration = 1.0F;
    return levelData;
}

static LevelData LevelData_2() {
}

static LevelData LevelData_3() {
}

static LevelData LevelData_4() {
}

static LevelData LevelData_5() {
}

LevelData GetLevelData(LevelId levelId) {
    switch (levelId) {
        case LevelId::LEVEL_1:
            return LevelData_1();
        case LevelId::LEVEL_2:
            return LevelData_2();
        case LevelId::LEVEL_3:
            return LevelData_3();
        case LevelId::LEVEL_4:
            return LevelData_4();
        case LevelId::LEVEL_5:
            return LevelData_5();
    }
}