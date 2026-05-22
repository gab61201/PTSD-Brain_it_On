#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include <functional>

#include "Util/GameObject.hpp"

namespace UI {

class Button : public Util::GameObject {
   public:
    Button(const std::shared_ptr<Core::Drawable>& drawable,
           const float zIndex,
           const glm::vec2& pivot = {0, 0},
           const bool visible = true);

    ~Button() = default;

    void Update();

    void OnClick(std::function<void()> OnClickHandler);

   private:
    std::function<void()> m_OnClickCallback;
    bool IsCursorPointing();
};

}  // namespace UI

#endif