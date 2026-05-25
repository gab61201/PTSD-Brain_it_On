#include "Screen/Button.hpp"

#include "Util/Input.hpp"

namespace UI {

Button::Button(
    const std::shared_ptr<Core::Drawable>& drawable,
    const float zIndex,
    const glm::vec2& pivot,
    const bool visible)
    : GameObject(drawable, zIndex, pivot, visible, {}) {}

void Button::Update() {
    if (m_OnClickCallback && IsCursorPointing() && Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        m_OnClickCallback();
    }
}

bool Button::IsCursorPointing() {
    glm::vec2 position = m_Transform.translation;
    glm::vec2 imageSize = GetScaledSize();
    glm::vec2 cursorPosition = Util::Input::GetCursorPosition();
    return (cursorPosition.x >= position.x - imageSize.x / 2.0f && cursorPosition.x <= position.x + imageSize.x / 2.0f &&
            cursorPosition.y >= position.y - imageSize.y / 2.0f && cursorPosition.y <= position.y + imageSize.y / 2.0f);
}

}  // namespace UI