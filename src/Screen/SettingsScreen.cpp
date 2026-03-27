#include "Screen/SettingsScreen.hpp"

namespace UI {

SettingsScreen::SettingsScreen() {
    auto background = UI::Element::CreateBackground("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "SettingsScreen"));
    m_Renderer.AddChild(title);
}

void SettingsScreen::Update() {
    m_Renderer.Update();
}

ScreenType SettingsScreen::GetNextScreenType() {
    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
        return ScreenType::MENU;
    }
    return ScreenType::LOBBY;
}

ScreenType SettingsScreen::GetScreenType() const {
    return ScreenType::SETTINGS;
}

}  // namespace UI