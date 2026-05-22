#include "Screen/SettingsScreen.hpp"

#include "Screen/UIElement.hpp"

namespace UI {

SettingsScreen::SettingsScreen() {
    auto background = UI::Element::Background("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("Resources/Fonts/Inter.ttf", 48, "SettingsScreen"));
    m_Renderer.AddChild(title);
}

ScreenType SettingsScreen::Update() {
    m_Renderer.Update();
    if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
        m_NextScreenType = ScreenType::MENU;
    }
    return m_NextScreenType;
}

ScreenType SettingsScreen::GetScreenType() const {
    return ScreenType::SETTINGS;
}

}  // namespace UI