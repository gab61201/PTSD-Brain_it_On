#include "Screen/LobbyScreen.hpp"

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

void LobbyScreen::Initialize() {
    AddGameObject(CreateTitle("TITLE"));
}

ScreenType LobbyScreen::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
        return ScreenType::MENU;
    }

    return ScreenType::LOBBY;
}

}  // namespace UI