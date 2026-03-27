#include "Screen/LobbyScreen.hpp"

namespace UI {

LobbyScreen::LobbyScreen() {
    m_NextScreenType = ScreenType::LOBBY;

    auto background = UI::Template::CreateBackground("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    auto titleTextShadow = std::make_shared<Util::Text>("Resources/Fonts/KaushanScript-Regular.ttf", 72, "Brain It On!", Util::Color::FromRGB(64, 64, 64));
    auto titleShadow = std::make_shared<Util::GameObject>(titleTextShadow, 0);
    titleShadow->m_Transform.translation = {3.0f, 197.0f};
    m_Renderer.AddChild(titleShadow);

    auto titleText = std::make_shared<Util::Text>("Resources/Fonts/KaushanScript-Regular.ttf", 72, "Brain It On!", Util::Color::FromRGB(255, 255, 255));
    auto title = std::make_shared<Util::GameObject>(titleText, 1);
    title->m_Transform.translation = {0.0f, 200.0f};
    m_Renderer.AddChild(title);

    auto playButtonImage = std::make_shared<Util::Image>("Resources/Images/Btn_MainButton_Gray.png");
    auto playButton = std::make_shared<UI::Button>(playButtonImage, 0);
    playButton->m_Transform.scale = {0.5f, 0.5f};
    playButton->OnClick([this](){
        m_NextScreenType = ScreenType::MENU;
    });
    m_Buttons.push_back(playButton);
    m_Renderer.AddChild(playButton);
}

void LobbyScreen::Update() {
    for (auto button : m_Buttons){
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