#include "Screen/Button.hpp"

namespace UI {

Button::Button(
    const std::shared_ptr<Core::Drawable>& drawable,
    const float zIndex,
    const glm::vec2& pivot,
    const bool visible)
    : GameObject(drawable, zIndex, pivot, visible, {}) {}

bool Button::IsCursorPointing() {
    glm::vec2 position = m_Transform.translation;
    glm::vec2 imageSize = GetScaledSize();
    glm::vec2 cursorPosition = Util::Input::GetCursorPosition();
    return (cursorPosition.x >= position.x - imageSize.x / 2.0f && cursorPosition.x <= position.x + imageSize.x / 2.0f &&
            cursorPosition.y >= position.y - imageSize.y / 2.0f && cursorPosition.y <= position.y + imageSize.y / 2.0f);
}

void Button::OnPressing() {
    if (IsCursorPointing() || Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        // scale to 1.1
    }
}

}  // namespace UI