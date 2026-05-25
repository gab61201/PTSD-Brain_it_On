#ifndef SCREEN_LOBBY_SCREEN_HPP
#define SCREEN_LOBBY_SCREEN_HPP

#include "Screen/UIScreen.hpp"

namespace UI {

class LobbyScreen : public UIScreen {
   public:
    LobbyScreen();

    ScreenType Update() override;

    ScreenType GetScreenType() const override;

   private:
    // 放需要設定偵測的物件
};

}  // namespace UI

#endif  // SCREEN_LOBBY_SCREEN_HPP