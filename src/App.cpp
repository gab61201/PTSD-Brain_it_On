#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    switch (ui.GetCurrentPhase()) {
        case UIManager::UIState::LOBBY:
            ui.Lobby();
            break;

        case UIManager::UIState::MENU:
            ui.Menu();
            break;

        case UIManager::UIState::GAME:
            ui.Game();
            break;

        case UIManager::UIState::SETTINGS:
            ui.Settings();
            break;

        default:
            break;
    }

    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() {  // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
