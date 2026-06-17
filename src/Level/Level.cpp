#include "Level/Level.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/ProgressStore.hpp"
#include "Util/Screenshot.hpp"
#include "Util/Time.hpp"

Level::Level(LevelId levelId) : m_LevelId(levelId) {
    LevelConfig data = GetLevelConfig(levelId);
    m_World = data.world;
    m_PassConditions = data.passConditions;
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
    bool allPassed = true;
    for (const auto& cond : m_PassConditions) {
        if (!cond->Check(m_World->GetContactEvents(), m_World->GetSensorEvents())) {
            allPassed = false;
        }
    }
    if (allPassed) {
        m_State = State::FINISHED;
        m_World->Stop();
    }
    int maxCountdown = 0;
    for (const auto& cond : m_PassConditions) {
        int cd = cond->GetContactCountDown();
        if (cd > maxCountdown) maxCountdown = cd;
    }
    m_HUD->UpdateContactTimer(maxCountdown);
}

void Level::Reset() {
    m_State = State::WAITING;
    LevelConfig data = GetLevelConfig(m_LevelId);
    m_World = data.world;
    m_PassConditions = data.passConditions;
    m_ElapsedTime = 0.0F;
    m_Timeout = data.timeout;
    m_StrokeLimit = data.strokeLimit;
    m_HUD->Reset(data.targetText, m_StrokeLimit);
}

void Level::Update() {
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
    m_World->Update();
    m_HUD->UpdateTimer(GetRemainingTime());
    m_HUD->UpdateStrokeLimit(m_World->GetDrawnObjectCount(), m_StrokeLimit);
    m_HUD->Update();

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
    m_LastResult.isNewRecord = Util::ProgressStore::ApplyResultAndSave(m_LastResult);
}
