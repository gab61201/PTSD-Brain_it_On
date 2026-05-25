#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "Screen/UIElement.hpp"

#include "Constants.hpp"

namespace UI::Element {

std::shared_ptr<Util::GameObject> Background(const std::string& path) {
    auto backgroundImage = std::make_shared<Util::Image>(path);  //"Resources/Images/background.png"
    auto background = std::make_shared<Util::GameObject>(backgroundImage, Layer::Background);
    glm::vec2 backgroundImageSize = backgroundImage->GetSize();
    background->m_Transform.scale = {RESOLUTION_X / backgroundImageSize.x,
                                     RESOLUTION_Y / backgroundImageSize.y};
    return background;
}

std::shared_ptr<UI::Button> Button(const std::string& path, std::function<void()> OnClickHandler) {
    auto buttonImage = std::make_shared<Util::Image>(path);
    auto button = std::make_shared<UI::Button>(buttonImage, Layer::UIElement);
    button->m_Transform.scale = {0.5f, 0.5f};
    button->OnClick(OnClickHandler);
    return button;
}

std::shared_ptr<Util::GameObject> Text(
    const std::string& text,
    int size,
    const glm::vec2& position,
    const Util::Color& color,
    float layer,
    const std::string& fontPath) {
    auto drawable = std::make_shared<Util::Text>(fontPath, size, text, color);
    auto object = std::make_shared<Util::GameObject>(drawable, layer);
    object->m_Transform.translation = position;
    return object;
}

std::shared_ptr<Util::GameObject> Image(
    const std::string& path,
    const glm::vec2& position,
    const glm::vec2& scale,
    float layer) {
    auto drawable = std::make_shared<Util::Image>(path);
    auto object = std::make_shared<Util::GameObject>(drawable, layer);
    object->m_Transform.translation = position;
    object->m_Transform.scale = scale;
    return object;
}

}  // namespace UI::Element