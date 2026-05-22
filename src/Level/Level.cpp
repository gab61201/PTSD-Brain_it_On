#include "Level/Level.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/ProgressStore.hpp"
#include "Util/Screenshot.hpp"
#include "Util/Time.hpp"

Level::Level(LevelId levelId) : m_LevelId(levelId) {
    LevelConfig data = GetLevelConfig(levelId);
    m_World = data.world;
    m_PassCondition = data.passCondition;
    m_Timeout = data.timeout;
    m_StrokeLimit = data.strokeLimit;
    m_HUD = std::make_unique<LevelHUD>(levelId, data.targetText,
                                       m_StrokeLimit);
}

void Level::Waiting() {
    // 檢查使用者是否開始繪圖
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        m_HUD->HideTarget();
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
    // 更新接觸倒數計時器
    int contactCountDown = m_PassCondition->GetContactCountDown();
    m_HUD->UpdateContactTimer(contactCountDown);
    // 檢查通關條件
    if (m_PassCondition->Check(m_World->GetContactEvents())) {
        m_state = State::FINISHED;
        Save();
        m_World->Stop();
    }
}

void Level::Reset() {
    m_state = State::WAITING;
    LevelConfig data = GetLevelConfig(m_LevelId);
    m_World = data.world;
    m_PassCondition = data.passCondition;
    m_Time = 0.0F;
    m_Timeout = data.timeout;
    m_StrokeLimit = data.strokeLimit;
    m_HUD->Reset(data.targetText, m_StrokeLimit);
}

void Level::Update() {
    // 只有在繪圖或播放物理模擬時才計時 (使用 GetDeltaTimeMs() 並除以 1000)
    if (m_state == State::DRAWING || m_state == State::PLAYING) {
        m_Time += static_cast<float>(Util::Time::GetDeltaTimeMs()) / 1000.0f;
    }

    // 繪製物體
    m_World->Update();

    // 更新 HUD（計時器、提示文字）
    m_HUD->UpdateTimer(GetRemainingTime());
    m_HUD->UpdateStrokeLimit(m_StrokeLimit - m_World->GetDrawnObjectCount(), m_StrokeLimit);
    m_HUD->Update();

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
    }
}

void Level::Save() {
    m_LastResult = LevelResult{
        m_LevelId,
        (m_state == State::FINISHED),
        m_Timeout,
        m_Time,
        m_StrokeLimit,
        m_World->GetDrawnObjectCount(),
        Util::Screenshot::Capture()
    };
    Util::ProgressStore::ApplyResultAndSave(m_LastResult);
}
