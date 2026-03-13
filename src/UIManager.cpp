#include "UIManager.hpp"

#include "Screen/GameScreen.hpp"
#include "Screen/LobbyScreen.hpp"
#include "Screen/MenuScreen.hpp"
#include "Screen/SettingsScreen.hpp"

std::unique_ptr<UI::UIScreen> UIManager::CreateScreen(UI::ScreenType screenType) {
    switch (screenType) {
        case UI::ScreenType::LOBBY:
            return std::make_unique<UI::LobbyScreen>();
        case UI::ScreenType::MENU:
            return std::make_unique<UI::MenuScreen>(m_Level);
        case UI::ScreenType::GAME:
            return std::make_unique<UI::GameScreen>(m_Level);
        case UI::ScreenType::SETTINGS:
            return std::make_unique<UI::SettingsScreen>();
    }

    return std::make_unique<UI::LobbyScreen>();
}

void UIManager::ChangeUI(UI::ScreenType nextUI) {
    if (m_CurrentScreen != nullptr) {
        m_CurrentScreen->Exit();
    }

    m_CurrentUI = nextUI;
    m_CurrentScreen = CreateScreen(m_CurrentUI);
    m_CurrentScreen->Initialize();
}

void UIManager::Update() {
    if (m_CurrentScreen == nullptr) {
        ChangeUI(m_CurrentUI);
    }

    const UI::ScreenType nextUI = m_CurrentScreen->Update();
    if (nextUI != m_CurrentUI) {
        ChangeUI(nextUI);
    }

    m_CurrentScreen->Render();
}