#include "Screen/UITemplate.hpp"

namespace UI::Template {

std::shared_ptr<Util::GameObject> CreateBackground(const std::string& path){
    auto backgroundImage = std::make_shared<Util::Image>(path); //"Resources/Images/background.png"
    auto background = std::make_shared<Util::GameObject>(backgroundImage, -1);
    glm::vec2 backgroundImageSize = backgroundImage->GetSize();
    background->m_Transform.scale = {RESOLUTION_X / backgroundImageSize.x,
                                     RESOLUTION_Y / backgroundImageSize.y};
    return background;
}


}  // namespace UI::Template