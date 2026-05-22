#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "Screen/Button.hpp"
#include "Util/Color.hpp"

namespace UI::Element {

std::shared_ptr<Util::GameObject> Background(const std::string& path);

std::shared_ptr<UI::Button> SquareButton(
    std::function<void()> OnClickHandler,
    const std::string& path = "Resources/Images/Btn_MainButton_Gray.png");

std::shared_ptr<UI::Button> CircleButton(
    std::function<void()> OnClickHandler,
    const std::string& path = "Resources/Images/Btn_OtherButton_Circle01.png");

std::shared_ptr<Util::GameObject> Text(
    const std::string& text,
    int size,
    const glm::vec2& position,
    const Util::Color& color,
    float z = 1.0f,
    const std::string& fontPath = "Resources/Fonts/Inter.ttf");

std::shared_ptr<Util::GameObject> Image(
    const std::string& path,
    const glm::vec2& position,
    const glm::vec2& scale,
    float z = 1.0f);

}  // namespace UI::Element

#endif
