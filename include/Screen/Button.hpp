#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include "Util/GameObject.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"

namespace UI {

class Button : public Util::GameObject {
   public:
    Button(const std::shared_ptr<Core::Drawable>& drawable,
           const float zIndex,
           const glm::vec2& pivot = {0, 0},
           const bool visible = true);

    ~Button() = default;

    template <typename Func>
    auto OnClick(Func func) {
        if (IsCursorPointing() && Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
            return func();
        }
    }

   private:
    bool IsCursorPointing();
    void OnPressing();
};

}  // namespace UI

#endif