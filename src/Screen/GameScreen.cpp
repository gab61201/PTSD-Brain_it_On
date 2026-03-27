#include "Screen/GameScreen.hpp"
#include <iostream>

namespace UI {

GameScreen::GameScreen(LevelId* levelId) : m_Level(*levelId) {
    m_NextScreenType = ScreenType::GAME;

    auto background = UI::Element::Background("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    auto backButton = UI::Element::CircleButton([this]{
        m_NextScreenType = ScreenType::MENU;
    });
    backButton->m_Transform.translation ={-550.0f, -300.0f};
    m_Buttons.push_back(backButton);
    m_Renderer.AddChild(backButton);

    auto resetButton = UI::Element::CircleButton([this]{
        m_Level.Reset();
    });
    resetButton->m_Transform.translation ={550.0f, -300.0f};
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