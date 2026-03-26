#include "Screen/LobbyScreen.hpp"

namespace UI {

LobbyScreen::LobbyScreen() {
    auto backgroundImage = std::make_shared<Util::Image>("Resources/Images/background.png");
    auto background = std::make_shared<Util::GameObject>(backgroundImage, -1);
    glm::vec2 backgroundImageSize = backgroundImage->GetSize();
    background->m_Transform.scale = {RESOLUTION_X / backgroundImageSize.x,
                                     RESOLUTION_Y / backgroundImageSize.y};
    m_Renderer.AddChild(background);

    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "LobbyScreen"));
    m_Renderer.AddChild(title);
}

void LobbyScreen::Update() {
    m_Renderer.Update();
}

ScreenType LobbyScreen::GetNextScreenType() const {
    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
        return ScreenType::MENU;
    }
    return ScreenType::LOBBY;
}

ScreenType LobbyScreen::GetScreenType() const {
    return ScreenType::LOBBY;
}

}  // namespace UI