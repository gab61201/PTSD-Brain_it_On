#include "Screen/UIScreen.hpp"

namespace UI::Element {

std::shared_ptr<Util::GameObject> Background(const std::string& path);
std::shared_ptr<UI::Button> SquareButton(std::function<void()> OnClickHandler);
std::shared_ptr<UI::Button> CircleButton(std::function<void()> OnClickHandler);

}  // namespace UI::Element