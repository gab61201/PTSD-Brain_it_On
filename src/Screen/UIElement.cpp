#include "Screen/UIElement.hpp"

namespace UI::Element {

std::shared_ptr<Util::GameObject> Background(const std::string& path) {
    auto backgroundImage = std::make_shared<Util::Image>(path);  //"Resources/Images/background.png"
    auto background = std::make_shared<Util::GameObject>(backgroundImage, -1);
    glm::vec2 backgroundImageSize = backgroundImage->GetSize();
    background->m_Transform.scale = {RESOLUTION_X / backgroundImageSize.x,
                                     RESOLUTION_Y / backgroundImageSize.y};
    return background;
}

std::shared_ptr<UI::Button> SquareButton(std::function<void()> OnClickHandler) {
    auto ButtonImage = std::make_shared<Util::Image>("Resources/Images/Btn_MainButton_Gray.png");
    auto Button = std::make_shared<UI::Button>(ButtonImage, 0);
    Button->m_Transform.scale = {0.5f, 0.5f};
    Button->OnClick(OnClickHandler);
    return Button;
}

std::shared_ptr<UI::Button> CircleButton(std::function<void()> OnClickHandler) {
    auto ButtonImage = std::make_shared<Util::Image>("Resources/Images/Btn_OtherButton_Circle01.png");
    auto Button = std::make_shared<UI::Button>(ButtonImage, 0);
    Button->m_Transform.scale = {0.5f, 0.5f};
    Button->OnClick(OnClickHandler);
    return Button;
}

}  // namespace UI::Element