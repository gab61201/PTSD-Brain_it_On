#include "GameScreen.hpp"

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

GameScreen::GameScreen(LevelManager& level)
    : m_Level(level) {}

void GameScreen::Enter() {
    AddGameObject(CreateTitle("Game"));
}

UIScreenType GameScreen::Update() {
    m_Level.Update();

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        return UIScreenType::SETTINGS;
    }

    return UIScreenType::GAME;
}