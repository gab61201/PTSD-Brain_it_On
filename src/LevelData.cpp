#include "LevelData.hpp"

LevelData GetLevelData(LevelId levelId) {
    switch (levelId) {
        case LevelId::LEVEL_1:
            return LevelData_1();
            break;
        case LevelId::LEVEL_2:
            return LevelData_2();
            break;
        case LevelId::LEVEL_3:
            return LevelData_3();
            break;
        case LevelId::LEVEL_4:
            return LevelData_4();
            break;
    }
}

static LevelData LevelData_1() {
}

static LevelData LevelData_2() {
}

static LevelData LevelData_3() {
}

static LevelData LevelData_4() {
}

static LevelData LevelData_5() {
}