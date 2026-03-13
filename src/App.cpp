#include "App.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    m_CurrentState = State::UPDATE;

    m_CurrentScreenType = UI::ScreenType::LOBBY;
    m_CurrentScreen = std::make_unique<UI::LobbyScreen>();
}

void App::Update() {
    UI::ScreenType nextUIType = m_CurrentScreen->Update();
    if (m_CurrentScreenType != nextUIType) {
        switch (nextUIType) {
            case UI::ScreenType::LOBBY:
                m_CurrentScreen = std::make_unique<UI::LobbyScreen>();
                break;
            case UI::ScreenType::SETTINGS:
                m_CurrentScreen = std::make_unique<UI::SettingsScreen>();
                break;
            case UI::ScreenType::MENU:
                m_CurrentScreen = std::make_unique<UI::MenuScreen>(&m_SelectedLevelId);
                break;
            case UI::ScreenType::GAME:
                m_CurrentScreen = std::make_unique<UI::GameScreen>(&m_SelectedLevelId);
                break;
        }
        m_CurrentScreenType = nextUIType;
    }

    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() {  // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
