#include "Screen/GameScreen.hpp"

namespace UI {

GameScreen::GameScreen(LevelId *levelId) {
    m_Level = Level(*levelId);
    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "GameScreen"));
    m_Renderer.AddChild(title);
}

ScreenType GameScreen::Update() {
    m_Renderer.Update();
    m_Level.Update();

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        return ScreenType::MENU;
    }

    return ScreenType::GAME;
}

}  // namespace UI