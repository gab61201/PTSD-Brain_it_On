#include "App.hpp"

#include "Screen/ResultScreen.hpp"
#include "Util/BGM.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    m_ProgressStore.LoadOrCreateDefault();

    Util::BGM("Resources/Audios/BGM.mp3").Play();

    m_CurrentState = State::UPDATE;

    m_ScreenType = UI::ScreenType::LOBBY;
    m_Screen = std::make_unique<UI::LobbyScreen>();
}

void App::Update() {
    const UI::ScreenType currentScreenType = m_Screen->GetScreenType();
    m_ScreenType = m_Screen->GetNextScreenType();
    if (m_ScreenType != currentScreenType) {
        switch (m_ScreenType) {
            case UI::ScreenType::LOBBY:
                m_Screen = std::make_unique<UI::LobbyScreen>();
                break;
            case UI::ScreenType::SETTINGS:
                m_Screen = std::make_unique<UI::SettingsScreen>();
                break;
            case UI::ScreenType::MENU:
                m_Screen = std::make_unique<UI::MenuScreen>(&m_SelectedLevelId, &m_ProgressStore);
                break;
            case UI::ScreenType::GAME:
                m_Screen = std::make_unique<UI::GameScreen>(&m_SelectedLevelId);
                break;
            case UI::ScreenType::RESULT:
                LevelResultData resultData{};
                if (currentScreenType != UI::ScreenType::GAME ||
                    !static_cast<UI::GameScreen*>(m_Screen.get())->TryGetResultData(&resultData)) {
                    LOG_WARN("TryGetResultData failed during RESULT transition, fallback to MENU");
                    m_Screen = std::make_unique<UI::MenuScreen>(&m_SelectedLevelId, &m_ProgressStore);
                    break;
                }

                {
                    const auto conditions = ProgressStore::CalculateConditions(resultData);
                    if (m_ProgressStore.UpdateBestStars(resultData.levelId, conditions)) {
                        if (!m_ProgressStore.Save()) {
                            LOG_WARN("Failed to save progress file");
                        }
                    }
                }

                m_Screen = std::make_unique<UI::ResultScreen>(&m_SelectedLevelId, resultData);
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
