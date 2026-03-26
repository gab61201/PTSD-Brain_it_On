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
    glm::vec2 size = m_Drawable->GetSize();
    glm::vec2 cursorPosition = Util::Input::GetCursorPosition();
    return (cursorPosition.x >= position.x - size.x && cursorPosition.x <= position.x + size.x &&
            cursorPosition.y >= position.y - size.y && cursorPosition.y <= position.y + size.y);
}

void Button::OnPressing() {
    if (IsCursorPointing() || Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        // scale to 1.1
    }
}

}  // namespace UI