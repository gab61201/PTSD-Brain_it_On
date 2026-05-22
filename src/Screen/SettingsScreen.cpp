#include "Screen/SettingsScreen.hpp"

#include "Screen/UIElement.hpp"
#include "Util/Input.hpp"
#include "Constants.hpp"

namespace UI {

SettingsScreen::SettingsScreen() {
    auto background = UI::Element::Background(Path::Background);
    m_Renderer.AddChild(background);

    auto title = UI::Element::Text("SettingsScreen", 48, {0.0f, 200.0f}, Util::Color::FromRGB(255, 255, 255), 1);
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