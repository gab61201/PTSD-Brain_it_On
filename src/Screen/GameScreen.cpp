#include "Screen/GameScreen.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Text.hpp"

namespace {
std::shared_ptr<Util::GameObject> CreateTitle(const std::string& text) {
    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, text));
    return title;
}
}  // namespace

namespace UI {

GameScreen::GameScreen(LevelId levelId) {
    AddGameObject(CreateTitle("Game"));
    // m_Level = Level(levelId);
}

ScreenType GameScreen::Update() {
    // m_Level.Update();

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        return ScreenType::MENU;
    }

    return ScreenType::GAME;
}

}  // namespace UI