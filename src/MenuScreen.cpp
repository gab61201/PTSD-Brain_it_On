#include "MenuScreen.hpp"

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

MenuScreen::MenuScreen(LevelManager& level)
    : m_Level(level) {}

void MenuScreen::Enter() {
    AddGameObject(CreateTitle("Menu"));
}

UIScreenType MenuScreen::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        m_Level.m_CurrentLevel = LevelManager::LevelState::LEVEL_1;
        return UIScreenType::GAME;
    }

    return UIScreenType::MENU;
}