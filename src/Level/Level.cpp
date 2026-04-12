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
    m_PassCondition = data.passCondition;
    m_Timeout = data.timeout;
    m_HUD = std::make_unique<LevelHUD>(levelId, data.targetText);
}

void Level::Waiting() {
    // 檢查使用者是否開始繪圖
    if (m_World && Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        m_HUD->HideTarget();
        m_state = State::DRAWING;
        m_World->Start();
        m_World->DrawObject(Util::Input::GetCursorPosition());
    }
}

void Level::Drawing() {
    if (!m_World) {
        return;
    }

    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        m_state = State::PLAYING;
        m_World->EndDrawing();
    } else {
        m_World->DrawObject(Util::Input::GetCursorPosition());
    }
}

void Level::Playing() {
    if (!m_World) {
        return;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        m_state = State::DRAWING;
        m_World->DrawObject(Util::Input::GetCursorPosition());
    }
    // 檢查通關條件
    if (m_PassCondition && m_PassCondition->Check(m_World->GetContactEvents())) {
        m_state = State::FINISHED;
        m_World->Stop();
    }
}

void Level::Finished() {
    // 預留：未來可在此觸發結算畫面
}

void Level::Reset() {
    m_state = State::WAITING;
    LevelData data = GetLevelData(m_LevelId);
    m_World = data.world;
    m_PassCondition = data.passCondition;
    m_Time = 0.0F;
    m_HUD->Reset(data.targetText);
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

    // 更新 HUD（計時器、提示文字）
    m_HUD->UpdateTimer(GetRemainingTime());
    m_HUD->Update();
}
