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
    if (m_World && Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        m_state = state::DRAWING;
        m_World->Start();
        m_World->DrawObject(Util::Input::GetCursorPosition());
    }
}

void Level::Drawing() {
    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        m_state = state::PLAYING;
        m_World->EndDrawing();
    } else {
        m_World->DrawObject(Util::Input::GetCursorPosition());
    }
}

void Level::Playing() {
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        m_state = state::DRAWING;
        m_World->DrawObject(Util::Input::GetCursorPosition());
    }
    // 檢查通關條件
    // for (auto& condition : m_pass_conditions) {
    //     if (!condition.Check()) {
    //         return;
    //     }
    // }
}

void Level::Finished() {
    // 停止世界，渲染結算畫面
}

void Level::Update() {
    switch (m_state) {
        case state::WAITING:
            Waiting();
            break;
        case state::DRAWING:
            Drawing();
            Playing();
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
