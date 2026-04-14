#ifndef UI_SCREEN_HPP
#define UI_SCREEN_HPP

#include "Screen/Button.hpp"
#include "Screen/UIElement.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"

#define RESOLUTION_X 1280.0F
#define RESOLUTION_Y 720.0F

namespace UI {

enum class ScreenType {
    LOBBY,
    SETTINGS,
    MENU,
    GAME,
    RESULT
};

class UIScreen {
   public:
    UIScreen() = default;

    virtual ~UIScreen() = default;

    // 渲染
    virtual void Update() = 0;

    // 取得下一個畫面的類型(放事件偵測)
    virtual ScreenType GetNextScreenType() = 0;

    virtual ScreenType GetScreenType() const = 0;

   protected:
    Util::Renderer m_Renderer;
    ScreenType m_NextScreenType;
    std::vector<std::shared_ptr<UI::Button>> m_Buttons;
};

}  // namespace UI

#endif