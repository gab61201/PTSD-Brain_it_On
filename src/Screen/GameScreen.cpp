#include "Screen/GameScreen.hpp"

namespace UI {

GameScreen::GameScreen(LevelId* levelId) : m_Level(*levelId) {
    auto background = UI::Element::Background("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    auto backButton = UI::Element::CircleButton([]{});
    backButton->m_Transform.translation ={-550.0f, -300.0f};
    m_Renderer.AddChild(backButton);

    auto resetButton = UI::Element::CircleButton([]{});
    resetButton->m_Transform.translation ={550.0f, -300.0f};
    m_Renderer.AddChild(resetButton);

    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "GameScreen"));
    m_Renderer.AddChild(title);
}

void GameScreen::Update() {
    m_Renderer.Update();
    m_Level.Update();
}

ScreenType GameScreen::GetNextScreenType() {
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        return ScreenType::MENU;
    }
    return ScreenType::GAME;
}

ScreenType GameScreen::GetScreenType() const {
    return ScreenType::GAME;
}

}  // namespace UI