#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "Screen/UIScreen.hpp"

namespace UI::Element {

std::shared_ptr<Util::GameObject> Background(const std::string& path);
std::shared_ptr<UI::Button> SquareButton(
    std::function<void()> OnClickHandler,
    const std::string& path = "Resources/Images/Btn_MainButton_Gray.png");
std::shared_ptr<UI::Button> CircleButton(
    std::function<void()> OnClickHandler,
    const std::string& path = "Resources/Images/Btn_OtherButton_Circle01.png");

}  // namespace UI::Element

#endif
