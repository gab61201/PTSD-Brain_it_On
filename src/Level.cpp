#include "Level.hpp"

Level::Level(LevelId levelId) : m_LevelId(levelId) {
    LevelData data = GetLevelData(levelId);
    m_Timeout = data.timeout;
    m_objects = std::move(data.objects);
    m_banned_areas = std::move(data.banned_areas);
    m_pass_conditions = std::move(data.pass_conditions);
    m_pass_condition_check_duration = data.pass_condition_check_duration;
}

void Level::Start() {
}

void Level::Update() {
}

void Level::End() {
}
