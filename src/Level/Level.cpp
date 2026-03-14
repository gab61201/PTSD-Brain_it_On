#include "Level/Level.hpp"

Level::Level(LevelId levelId) : m_LevelId(levelId) {
    LevelData data = GetLevelData(levelId);
    m_Timeout = data.timeout;
    m_objects = std::move(data.objects);
    m_banned_areas = std::move(data.banned_areas);
    m_pass_conditions = std::move(data.pass_conditions);
    m_pass_condition_check_duration = data.pass_condition_check_duration;
}

void Level::Waiting() {
    // 檢查使用者是否開始繪圖
}

void Level::Playing() {
    // 檢查通關條件
    for (auto& condition : m_pass_conditions) {
        if (!condition.Check()) {
            return;
        }
    }
}

void Level::Finished() {
    // 渲染結算畫面
}

void Level::Update() {
    switch (m_state) {
        case state::WAITING:
            Waiting();
            break;
        case state::PLAYING:
            Playing();
            break;
        case state::FINISHED:
            Finished();
            break;
    }
    // 繪製物體
    for (auto& object : m_objects) {
        object->Sync();
    }
    // 繪製區域
    for (auto& area : m_banned_areas) {
        area->Draw();
    }
}
