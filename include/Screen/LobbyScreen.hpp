#ifndef LOBBY_SCREEN_HPP
#define LOBBY_SCREEN_HPP

#include "Screen/UIScreen.hpp"

namespace UI {

class LobbyScreen : public UIScreen {
   public:
    LobbyScreen();

    void Update() override;

    ScreenType GetNextScreenType() const override;

   private:
    // 放需要設定偵測的物件
};

}  // namespace UI

#endif