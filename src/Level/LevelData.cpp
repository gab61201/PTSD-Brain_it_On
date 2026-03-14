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
    return LevelData{};
}

static LevelData LevelData_3() {
    return LevelData{};
}

static LevelData LevelData_4() {
    return LevelData{};
}

static LevelData LevelData_5() {
    return LevelData{};
}

LevelData GetLevelData(LevelId levelId) {
    static constexpr LevelData (*level_funcs[])() = {
        LevelData_1,
        LevelData_2,
        LevelData_3,
        LevelData_4,
        LevelData_5
    };

    auto index = static_cast<std::size_t>(levelId);
    if (index < std::size(level_funcs)) {
        return level_funcs[index]();
    }

    return LevelData{};
}
