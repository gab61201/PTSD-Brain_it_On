#include "App.hpp"

#include "Util/ProgressStore.hpp"
#include "Screen/ResultScreen.hpp"
#include "Util/BGM.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");
    Util::ProgressStore::LoadOrCreateDefault();
    Util::BGM("Resources/Audios/BGM.mp3").Play();
    m_CurrentState = State::UPDATE;
    m_Screen = std::make_unique<UI::LobbyScreen>();
}

void App::Update() {
    UI::ScreenType nextScreenType = m_Screen->Update();
    if (nextScreenType != m_Screen->GetScreenType()) {
        switch (nextScreenType) {
            case UI::ScreenType::LOBBY:
                m_Screen = std::make_unique<UI::LobbyScreen>();
                break;
            case UI::ScreenType::SETTINGS:
                m_Screen = std::make_unique<UI::SettingsScreen>();
                break;
            case UI::ScreenType::MENU:
                m_Screen = std::make_unique<UI::MenuScreen>();
                break;
            case UI::ScreenType::GAME:
                m_SelectedLevelId = static_cast<UI::MenuScreen*>(m_Screen.get())->GetSelectedLevelId();
                m_Screen = std::make_unique<UI::GameScreen>(m_SelectedLevelId);
                break;
            case UI::ScreenType::RESULT:
                const LevelResultData resultData = static_cast<UI::GameScreen*>(m_Screen.get())->GetResultData();
                Util::ProgressStore::ApplyResultAndSave(resultData);
                m_Screen = std::make_unique<UI::ResultScreen>(&m_SelectedLevelId, resultData);
                break;
        }
    }

    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() {  // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
