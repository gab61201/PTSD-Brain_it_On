#ifndef UI_SCREEN_HPP
#define UI_SCREEN_HPP

#include "Screen/Button.hpp"
#include "Util/Renderer.hpp"

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

    // 渲染並取得下一個畫面的類型(放事件偵測)
    virtual ScreenType Update() = 0;

    virtual ScreenType GetScreenType() const = 0;

   protected:
    Util::Renderer m_Renderer;
    ScreenType m_NextScreenType;
    std::vector<std::shared_ptr<UI::Button>> m_Buttons;
};

}  // namespace UI

#endif