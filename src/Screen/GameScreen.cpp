#include "Screen/GameScreen.hpp"

#include "Core/Context.hpp"
#include "Util/Image.hpp"
#include "Level/LevelData.hpp"
#include "Util/Keycode.hpp"

namespace UI {

GameScreen::GameScreen(LevelId* levelId) : m_Level(*levelId) {
    m_NextScreenType = ScreenType::GAME;

    auto background = UI::Element::Background("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    // 返回按鈕
    auto backButton = UI::Element::CircleButton([this]{
        m_NextScreenType = ScreenType::MENU;
    }, "Resources/Images/Btn_Back.png");
    backButton->m_Transform.translation ={-560.0f, -300.0f};
    m_Buttons.push_back(backButton);
    m_Renderer.AddChild(backButton);

    // 重試按鈕
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

    if (m_Level.GetState() == Level::State::FINISHED) {
        Core::Context::TakeScreenshot(static_cast<int>(m_Level.GetLevelId()));
        m_NextScreenType = ScreenType::RESULT;
        return;
    }

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

LevelResultData GameScreen::GetResultData() const {
    return m_Level.GetResultData();
}

}  // namespace UI