#include "Screen/GameScreen.hpp"
#include <iostream>
#include "Util/Image.hpp"
#include "Util/Text.hpp"

namespace UI {

GameScreen::GameScreen(LevelId* levelId) : m_Level(*levelId) {
    m_NextScreenType = ScreenType::GAME;

    auto background = UI::Element::Background("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    // ==========================================
    // 左側 UI 靜態圖示 (精密微調)
    // ==========================================
    float leftUI_X = -520.0f; // 往右移一點，避免貼邊

    // 1. 關卡編號外框
    auto levelFrameImage = std::make_shared<Util::Image>("Resources/Images/level_frame.png");
    auto levelFrame = std::make_shared<Util::GameObject>(levelFrameImage, 0.1f);
    levelFrame->m_Transform.translation = {leftUI_X, 280.0f};
    levelFrame->m_Transform.scale = {0.2f, 0.2f};
    m_Renderer.AddChild(levelFrame);

    // 2. 計時器鬧鐘圖示
    auto alarmImage = std::make_shared<Util::Image>("Resources/Images/alarm.png");
    auto alarmIcon = std::make_shared<Util::GameObject>(alarmImage, 0.1f);
    alarmIcon->m_Transform.translation = {leftUI_X, 160.0f}; // 往下移，拉開距離
    alarmIcon->m_Transform.scale = {0.12f, 0.12f}; // 縮小一點，更精緻
    m_Renderer.AddChild(alarmIcon);

    auto backButton = UI::Element::CircleButton([this]{
        m_NextScreenType = ScreenType::MENU;
    }, "Resources/Images/Btn_Back.png");
    backButton->m_Transform.translation ={-560.0f, -300.0f};
    m_Buttons.push_back(backButton);
    m_Renderer.AddChild(backButton);

    auto resetButton = UI::Element::CircleButton([this]{
        m_Level.Reset();
    }, "Resources/Images/Btn_Retry.png");
    resetButton->m_Transform.translation ={560.0f, -300.0f};
    m_Buttons.push_back(resetButton);
    m_Renderer.AddChild(resetButton);
}

void GameScreen::Update() {
    for (auto button : m_Buttons) {
        button->Update();
    }
    m_Renderer.Update();
    m_Level.Update();
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_NextScreenType = ScreenType::MENU;
    }
}

ScreenType GameScreen::GetNextScreenType() {
    return m_NextScreenType;
}

ScreenType GameScreen::GetScreenType() const {
    return ScreenType::GAME;
}

}  // namespace UI