#include "App.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/BGM.hpp"

void App::Start() {
    LOG_TRACE("Start");

    Util::BGM bgm("Resources/Audios/BGM.mp3");
    bgm.Play();

    m_CurrentState = State::UPDATE;

    m_ScreenType = UI::ScreenType::LOBBY;
    m_Screen = std::make_unique<UI::LobbyScreen>();
}

void App::Update() {
    m_ScreenType = m_Screen->GetNextScreenType();
    if (m_ScreenType != m_Screen->GetScreenType()) {
        switch (m_ScreenType) {
            case UI::ScreenType::LOBBY:
                m_Screen = std::make_unique<UI::LobbyScreen>();
                break;
            case UI::ScreenType::SETTINGS:
                m_Screen = std::make_unique<UI::SettingsScreen>();
                break;
            case UI::ScreenType::MENU:
                m_Screen = std::make_unique<UI::MenuScreen>(&m_SelectedLevelId);
                break;
            case UI::ScreenType::GAME:
                m_Screen = std::make_unique<UI::GameScreen>(&m_SelectedLevelId);
                break;
        }
        m_ScreenType = m_Screen->GetScreenType();
    }
    m_Screen->Update();

    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() {  // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
