#include "App.hpp"

#include "Screen/GameScreen.hpp"
#include "Screen/LobbyScreen.hpp"
#include "Screen/MenuScreen.hpp"
#include "Screen/ResultScreen.hpp"
#include "Screen/SettingsScreen.hpp"
#include "Util/BGM.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"
#include "Util/ProgressStore.hpp"

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
                m_Screen = std::make_unique<UI::ResultScreen>(&m_SelectedLevelId);
                break;
        }
    }

    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() {  // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
    Util::ProgressStore::CleanUpUnusedScreenshots();
}
