#include "Level/Level.hpp"

#include <iomanip>
#include <sstream>

#include "Util/Color.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Text.hpp"
#include "Util/Time.hpp"

Level::Level(LevelId levelId) : m_LevelId(levelId) {
    LevelData data = GetLevelData(levelId);
    m_World = data.world;
    m_Timeout = data.timeout;
}

void Level::Waiting() {
    // 檢查使用者是否開始繪圖
    if (m_World && Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        m_state = State::DRAWING;
        m_World->Start();
        m_World->DrawObject(Util::Input::GetCursorPosition());
    }
}

void Level::Drawing() {
    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        m_state = State::PLAYING;
        m_World->EndDrawing();
    } else {
        m_World->DrawObject(Util::Input::GetCursorPosition());
    }
}

void Level::Playing() {
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        m_state = State::DRAWING;
        m_World->DrawObject(Util::Input::GetCursorPosition());
    }
    // 檢查通關條件
    if (m_World->IsPassed()) {
        m_state = State::FINISHED;
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

void Level::Reset() {
    m_state = State::WAITING;
    m_World = GetLevelData(m_LevelId).world;
    m_Time = 0.0F;
}

void Level::Update() {
    // 只有在繪圖或播放物理模擬時才計時 (使用 GetDeltaTimeMs() 並除以 1000)
    if (m_state == State::DRAWING || m_state == State::PLAYING) {
        m_Time += static_cast<float>(Util::Time::GetDeltaTimeMs()) / 1000.0f;
    }

    switch (m_state) {
        case State::WAITING:
            Waiting();
            break;
        case State::DRAWING:
            Drawing();
            Playing();
            break;
        case State::PLAYING:
            Playing();
            break;
        case State::FINISHED:
            Finished();
            break;
    }

    // 繪製物體
    if (m_World) {
        m_World->Update();
    }
}
