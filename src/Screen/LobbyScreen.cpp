#include "Screen/LobbyScreen.hpp"

#include "Screen/UIElement.hpp"
#include "Constants.hpp"

namespace UI {

LobbyScreen::LobbyScreen() : UIScreen(ScreenType::LOBBY) {

    auto background = UI::Element::Background(Path::Background);
    m_Renderer.AddChild(background);

    auto title = UI::Element::Text(
        "Brain It On!", 72, {0.0f, 200.0f}, Util::Color::FromRGB(255, 255, 255), Layer::UIElement, "Resources/Fonts/KaushanScript-Regular.ttf");
    m_Renderer.AddChild(title);

    auto titleShadow = UI::Element::Text(
        "Brain It On!", 72, {3.0f, 197.0f}, Util::Color::FromRGB(64, 64, 64), Layer::UIOutline, "Resources/Fonts/KaushanScript-Regular.ttf");
    m_Renderer.AddChild(titleShadow);

    auto playButton = UI::Element::Button(Path::BtnMainButtonGray, [this]() {
        m_NextScreenType = ScreenType::MENU;
    });
    m_Buttons.push_back(playButton);
    m_Renderer.AddChild(playButton);
}

ScreenType LobbyScreen::Update() {
    m_Renderer.Update();
    for (auto button : m_Buttons) {
        button->Update();
    }
    return m_NextScreenType;
}

}  // namespace UI