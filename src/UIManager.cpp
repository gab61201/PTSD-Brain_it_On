#include "UIManager.hpp"

#include "GameScreen.hpp"
#include "LobbyScreen.hpp"
#include "MenuScreen.hpp"
#include "SettingsScreen.hpp"

std::unique_ptr<UIScreen> UIManager::CreateScreen(UIScreenType screenType) {
    switch (screenType) {
        case UIScreenType::LOBBY:
            return std::make_unique<LobbyScreen>();
        case UIScreenType::MENU:
            return std::make_unique<MenuScreen>(m_Level);
        case UIScreenType::GAME:
            return std::make_unique<GameScreen>(m_Level);
        case UIScreenType::SETTINGS:
            return std::make_unique<SettingsScreen>();
    }

    return std::make_unique<LobbyScreen>();
}

void UIManager::ChangeUI(UIScreenType nextUI) {
    if (m_CurrentScreen != nullptr) {
        m_CurrentScreen->Exit();
    }

    m_CurrentUI = nextUI;
    m_CurrentScreen = CreateScreen(m_CurrentUI);
    m_CurrentScreen->Enter();
}

void UIManager::Update() {
    if (m_CurrentScreen == nullptr) {
        ChangeUI(m_CurrentUI);
    }

    const UIScreenType nextUI = m_CurrentScreen->Update();
    if (nextUI != m_CurrentUI) {
        ChangeUI(nextUI);
    }

    m_CurrentScreen->Render();
}