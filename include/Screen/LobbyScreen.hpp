#ifndef LOBBY_SCREEN_HPP
#define LOBBY_SCREEN_HPP

#include "Screen/UIScreen.hpp"

namespace UI {

class LobbyScreen : public UIScreen {
   public:
    LobbyScreen();

    ScreenType Update() override;

    ScreenType GetScreenType() const override { return ScreenType::LOBBY; };

   private:
    // 放需要設定偵測的物件
};

}  // namespace UI

#endif