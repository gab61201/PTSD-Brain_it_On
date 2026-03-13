#ifndef LEVEL_DATA_HPP
#define LEVEL_DATA_HPP

#include <map>

enum class LevelId {
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
};

struct LevelData {
    float timeout;
    // std::vector<ObjectData> initialObjects; // 例如所有箱子的座標、長寬
    // PassConditionType condition;
};

const std::map<LevelId, LevelData>& GetAllLevelData();

#endif
