#include "UIManager.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Text.hpp"
#include "Util/TransformUtils.hpp"

// UI 切換器
void UIManager::Update() {
    UIState last_ui = m_CurrentUI;
    switch (m_CurrentUI) {
        case UIState::LOBBY:
            RenderLobby();
            break;
        case UIState::MENU:
            RenderMenu();
            break;
        case UIState::GAME:
            Game();
            break;
        case UIState::SETTINGS:
            RenderSettings();
            break;
        default:
            break;
    }
    // 若切換 UI，清除暫存
    if (last_ui != m_CurrentUI) {
        m_GameObjects.clear();
        m_Drawables.clear();
    }
}


void UIManager::RenderLobby() {
    // 建立 UI 物件
    if (m_GameObjects.empty()) {
        auto title = std::make_shared<Util::GameObject>();
        title->SetDrawable(std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "TITLE"));
        m_GameObjects.push_back(title);
    }
    // 渲染 UI 物件
    for (auto object : m_GameObjects) {
        object->Draw();
    }
    // 偵測事件並切換 UI
    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
        m_CurrentUI = UIState::MENU;
    }
}

void UIManager::RenderMenu() {
    // 建立 UI 物件
    if (m_GameObjects.empty()) {
        auto title = std::make_shared<Util::GameObject>();
        title->SetDrawable(std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "Menu"));
        m_GameObjects.push_back(title);
    }
    // 渲染 UI 物件
    for (auto object : m_GameObjects) {
        object->Draw();
    }
    // 偵測事件並切換 UI
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        level.m_CurrentLevel = LevelManager::LevelState::LEVEL_1;
        m_CurrentUI = UIState::GAME;
    }
}

void UIManager::RenderSettings() {
    // 建立 UI 物件
    if (m_GameObjects.empty()) {
        auto title = std::make_shared<Util::GameObject>();
        title->SetDrawable(std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "Settings"));
        m_GameObjects.push_back(title);
    }
    // 渲染 UI 物件
    for (auto object : m_GameObjects) {
        object->Draw();
    }
    // 偵測事件並切換 UI
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_CurrentUI = UIState::GAME;
    }
}

void UIManager::Game() {
    // 建立 UI 物件
    if (m_GameObjects.empty()) {
        auto title = std::make_shared<Util::GameObject>();
        title->SetDrawable(std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "Game"));
        m_GameObjects.push_back(title);
    }
    // 渲染 UI 物件
    for (auto object : m_GameObjects) {
        object->Draw();
    }
    // 偵測事件並切換 UI
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_CurrentUI = UIState::SETTINGS;
    }
}