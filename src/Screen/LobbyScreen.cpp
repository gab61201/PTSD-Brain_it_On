#include "Screen/LobbyScreen.hpp"

namespace UI {

LobbyScreen::LobbyScreen() {
    auto backgroundImage = std::make_shared<Util::Image>("Resources/Images/background.png");
    auto background = std::make_shared<Util::GameObject>(backgroundImage, -1);
    glm::vec2 backgroundImageSize = backgroundImage->GetSize();
    background->m_Transform.scale = {RESOLUTION_X / backgroundImageSize.x,
                                     RESOLUTION_Y / backgroundImageSize.y};
    m_Renderer.AddChild(background);

    
    auto titleTextShadow = std::make_shared<Util::Text>("Resources/Fonts/KaushanScript-Regular.ttf", 72, "Brain It On!", Util::Color::FromRGB(0, 0, 0));
    auto titleShadow = std::make_shared<Util::GameObject>(titleTextShadow, 0);
    titleShadow->m_Transform.translation = {1.0f, 197.0f};
    m_Renderer.AddChild(titleShadow);

    auto titleText = std::make_shared<Util::Text>("Resources/Fonts/KaushanScript-Regular.ttf", 72, "Brain It On!", Util::Color::FromRGB(255, 255, 255));
    auto title = std::make_shared<Util::GameObject>(titleText, 1);
    title->m_Transform.translation = {0.0f, 200.0f};
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