#include "Screen/GameScreen.hpp"

namespace UI {

GameScreen::GameScreen(LevelId* levelId) : m_Level(*levelId) {
    m_Renderer.AddChildren(m_Level.GetVisuals());

    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "GameScreen"));
    m_Renderer.AddChild(title);
}

void GameScreen::Update() {
    m_Renderer.Update();
    m_Level.Update();
}

ScreenType GameScreen::GetNextScreenType() const {
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        return ScreenType::MENU;
    }
    return ScreenType::GAME;
}

}  // namespace UI