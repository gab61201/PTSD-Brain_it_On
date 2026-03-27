#include "Screen/LobbyScreen.hpp"

namespace {

std::shared_ptr<Util::GameObject> Title() {
    auto titleShadowText = std::make_shared<Util::Text>("Resources/Fonts/KaushanScript-Regular.ttf", 72, "Brain It On!", Util::Color::FromRGB(64, 64, 64));
    auto titleShadow = std::make_shared<Util::GameObject>(titleShadowText, 0);
    titleShadow->m_Transform.translation = {3.0f, 197.0f};

    auto titleText = std::make_shared<Util::Text>("Resources/Fonts/KaushanScript-Regular.ttf", 72, "Brain It On!", Util::Color::FromRGB(255, 255, 255));
    auto title = std::make_shared<Util::GameObject>(titleText, 1);
    title->m_Transform.translation = {0.0f, 200.0f};
    title->AddChild(titleShadow);
    return title;
}

}  // namespace

namespace UI {

LobbyScreen::LobbyScreen() {
    m_NextScreenType = ScreenType::LOBBY;

    auto background = UI::Element::Background("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    auto title = Title();
    m_Renderer.AddChild(title);

    auto playButton = UI::Element::SquareButton([this]() {
        m_NextScreenType = ScreenType::MENU;
    });
    m_Buttons.push_back(playButton);
    m_Renderer.AddChild(playButton);
}

void LobbyScreen::Update() {
    for (auto button : m_Buttons) {
        button->Update();
    }
    m_Renderer.Update();
}

ScreenType LobbyScreen::GetNextScreenType() {
    return m_NextScreenType;
}

ScreenType LobbyScreen::GetScreenType() const {
    return ScreenType::LOBBY;
}

}  // namespace UI