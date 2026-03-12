#include "UIManager.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "Util/TransformUtils.hpp"

void UIManager::Lobby() {
    // Draw Lobby UI Here
    if (m_GameObjects.empty()){
        auto title = std::make_shared<Util::GameObject>();
        title->SetDrawable(std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "TITLE"));
        m_GameObjects.push_back(title);
    }
    for (auto object : m_GameObjects) {
        object->Draw();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
        m_GameObjects.clear();
        m_Drawables.clear();
        m_CurrentPhase = UIState::MENU;
    }
}

void UIManager::Menu() {
    // Draw Menu UI Here
    if (m_GameObjects.empty()){
        auto title = std::make_shared<Util::GameObject>();
        title->SetDrawable(std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "Menu"));
        m_GameObjects.push_back(title);
    }
    for (auto object : m_GameObjects) {
        object->Draw();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
        m_GameObjects.clear();
        m_Drawables.clear();
        m_CurrentPhase = UIState::GAME;
    }
}

void UIManager::Game() {
    // Draw Game UI Here
    if (m_GameObjects.empty()){
        auto title = std::make_shared<Util::GameObject>();
        title->SetDrawable(std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "Game"));
        m_GameObjects.push_back(title);
    }
    for (auto object : m_GameObjects) {
        object->Draw();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_GameObjects.clear();
        m_Drawables.clear();
        m_CurrentPhase = UIState::SETTINGS;
    }
}

void UIManager::Settings() {
    // Draw Settings UI Here
    if (m_GameObjects.empty()){
        auto title = std::make_shared<Util::GameObject>();
        title->SetDrawable(std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "Settings"));
        m_GameObjects.push_back(title);
    }
    for (auto object : m_GameObjects) {
        object->Draw();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_GameObjects.clear();
        m_Drawables.clear();
        m_CurrentPhase = UIState::GAME;
    }
}