#include "LobbyScreen.hpp"

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

void LobbyScreen::Enter() {
    AddGameObject(CreateTitle("TITLE"));
}

UIScreenType LobbyScreen::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
        return UIScreenType::MENU;
    }

    return UIScreenType::LOBBY;
}