#pragma once

#include "Constants.hpp"
#include "Screen/Button.hpp"
#include "Util/Color.hpp"

namespace UI::Element {

std::shared_ptr<Util::GameObject> Background(const std::string& path);

std::shared_ptr<UI::Button> Button(
    const std::string& path,
    std::function<void()> OnClickHandler);

std::shared_ptr<Util::GameObject> Text(
    const std::string& text,
    int size,
    const glm::vec2& position,
    const Util::Color& color,
    float z = Layer::UIElement,
    const std::string& fontPath = "Resources/Fonts/Inter.ttf");

std::shared_ptr<Util::GameObject> Image(
    const std::string& path,
    const glm::vec2& position,
    const glm::vec2& scale,
    float z = Layer::UIElement);

}  // namespace UI::Element
