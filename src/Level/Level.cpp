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
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        m_HUD->HideTargetText();
        m_State = State::PLAYING;
        m_World->Start();
        m_World->DrawNewObject(Util::Input::GetCursorPosition());
    }
}

void Level::Playing() {
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        m_World->DrawNewObject(Util::Input::GetCursorPosition());
    } else if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        m_World->DrawingObject(Util::Input::GetCursorPosition());
    } else if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        m_World->EndDrawing();
    }
    m_ElapsedTime += static_cast<float>(Util::Time::GetDeltaTimeMs()) / 1000.0f;
    int contactCountDown = m_PassCondition->GetContactCountDown();
    m_HUD->UpdateContactTimer(contactCountDown);
    if (m_PassCondition->Check(m_World->GetContactEvents())) {
        m_State = State::FINISHED;
        m_World->Stop();
        Save();
    }
}

void Level::Reset() {
    m_State = State::WAITING;
    LevelConfig data = GetLevelConfig(m_LevelId);
    m_World = data.world;
    m_PassCondition = data.passCondition;
    m_ElapsedTime = 0.0F;
    m_Timeout = data.timeout;
    m_StrokeLimit = data.strokeLimit;
    m_HUD->Reset(data.targetText, m_StrokeLimit);
}

void Level::Update() {
    m_World->Update();

    m_HUD->UpdateTimer(GetRemainingTime());
    m_HUD->UpdateStrokeLimit(m_StrokeLimit - m_World->GetDrawnObjectCount(), m_StrokeLimit);
    m_HUD->Update();

    switch (m_State) {
        case State::WAITING:
            Waiting();
            break;
        case State::PLAYING:
            Playing();
            break;
        case State::FINISHED:
            break;
    }
}

void Level::Save() {
    m_LastResult = LevelResult{
        m_LevelId,
        (m_State == State::FINISHED),
        m_Timeout,
        m_ElapsedTime,
        m_StrokeLimit,
        m_World->GetDrawnObjectCount(),
        Util::Screenshot::Capture()};
    m_IsNewRecord = Util::ProgressStore::ApplyResultAndSave(m_LastResult);
}
