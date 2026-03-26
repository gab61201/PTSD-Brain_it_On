#ifndef UI_SCREEN_HPP
#define UI_SCREEN_HPP

#include "Util/GameObject.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"

namespace UI {

enum class ScreenType {
    LOBBY,
    SETTINGS,
    MENU,
    GAME
};

class UIScreen {
   public:
    UIScreen() = default;

    virtual ~UIScreen() = default;

    // 渲染
    virtual void Update() = 0;

    // 取得下一個畫面的類型(放事件偵測)
    virtual ScreenType GetNextScreenType() const = 0;

    virtual ScreenType GetScreenType() const = 0;

   protected:
    Util::Renderer m_Renderer;
};

}  // namespace UI

#endif