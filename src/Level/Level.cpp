#include "Level/Level.hpp"
#include "Util/Text.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

Level::Level(LevelId levelId) : m_LevelId(levelId) {
    LevelData data = GetLevelData(levelId);
    m_World = data.world;
    m_Timeout = data.timeout;
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
    if (m_World->IsPassed()) {
        m_state = state::FINISHED;
        m_World->Stop();
    }
}

void Level::Finished() {
    // 停止世界，渲染結算畫面
    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "Finished"));
    title->Draw();
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
