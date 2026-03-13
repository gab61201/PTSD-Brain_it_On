#include "Screen/LobbyScreen.hpp"

namespace UI {

LobbyScreen::LobbyScreen() {
    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "LobbyScreen"));
    m_Renderer.AddChild(title);
}

ScreenType LobbyScreen::Update() {
    m_Renderer.Update();

    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
        return ScreenType::MENU;
    }

    return ScreenType::LOBBY;
}

}  // namespace UI