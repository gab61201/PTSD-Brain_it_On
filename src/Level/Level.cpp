#include "Level/Level.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

Level::Level(LevelId levelId) : m_LevelId(levelId) {
    LevelData data = GetLevelData(levelId);
    m_World = data.world;
    m_Timeout = data.timeout;
    m_pass_conditions = std::move(data.pass_conditions);
    m_pass_condition_check_duration = data.pass_condition_check_duration;
}

void Level::Waiting() {
    // 檢查使用者是否開始繪圖
    if (m_World && Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        m_state = state::PLAYING;
        m_World->Start();
    }
}

void Level::Playing() {
    // 檢查通關條件
    // for (auto& condition : m_pass_conditions) {
    //     if (!condition.Check()) {
    //         return;
    //     }
    // }
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
    if (m_World) {
        m_World->Update();
    }
}
