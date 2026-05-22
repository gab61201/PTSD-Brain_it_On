#include "App.hpp"

#include <filesystem>

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
        if (m_Screen->GetScreenType() == UI::ScreenType::RESULT && !m_IsNewRecord) {
            std::error_code ec;
            std::filesystem::remove("Resources/Save/Screenshots/" + m_LastScreenshotFilename, ec);
        }
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
                if (auto* menu = dynamic_cast<UI::MenuScreen*>(m_Screen.get())) {
                    m_SelectedLevelId = menu->GetSelectedLevelId();
                } else if (auto* result = dynamic_cast<UI::ResultScreen*>(m_Screen.get())) {
                    m_SelectedLevelId = result->GetNextLevelId();
                }
                m_Screen = std::make_unique<UI::GameScreen>(m_SelectedLevelId);
                break;
            case UI::ScreenType::RESULT:
                if (auto* game = dynamic_cast<UI::GameScreen*>(m_Screen.get())) {
                    m_LastResult = game->GetLastResult();
                    m_IsNewRecord = game->IsNewRecord();
                    m_LastScreenshotFilename = m_LastResult.screenshotFilename;
                }
                m_Screen = std::make_unique<UI::ResultScreen>(m_LastResult);
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
