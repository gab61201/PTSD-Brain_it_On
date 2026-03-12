#include "Screen/MenuScreen.hpp"

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

MenuScreen::MenuScreen(LevelManager& level)
    : m_Level(level) {}

void MenuScreen::Enter() {
    AddGameObject(CreateTitle("Menu"));
}

ScreenType MenuScreen::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        m_Level.m_CurrentLevel = LevelManager::LevelState::LEVEL_1;
        return ScreenType::GAME;
    }

    return ScreenType::MENU;
}

}  // namespace UI