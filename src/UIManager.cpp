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
            return std::make_unique<UI::MenuScreen>();
        case UI::ScreenType::GAME:
            return std::make_unique<UI::GameScreen>(m_SelectedLevelId);
        case UI::ScreenType::SETTINGS:
            return std::make_unique<UI::SettingsScreen>();
    }

    return std::make_unique<UI::LobbyScreen>();
}

void UIManager::ChangeUI(UI::ScreenType nextUIType) {
    if (m_CurrentScreen != nullptr) {
        m_CurrentScreen->Exit();
    }

    m_CurrentScreenType = nextUIType;
    m_CurrentScreen = CreateScreen(m_CurrentScreenType);
}

void UIManager::Update() {
    if (m_CurrentScreen == nullptr) {
        ChangeUI(m_CurrentScreenType);
    }

    const UI::ScreenType nextUIType = m_CurrentScreen->Update();
    if (m_CurrentScreenType == UI::ScreenType::MENU &&
        nextUIType == UI::ScreenType::GAME) {
        auto* menu = dynamic_cast<UI::MenuScreen*>(m_CurrentScreen.get());
        if (menu != nullptr) {
            m_SelectedLevelId = menu->GetSelectedLevelType();
        }
    }

    if (nextUIType != m_CurrentScreenType) {
        ChangeUI(nextUIType);
    }

    m_CurrentScreen->Render();
}