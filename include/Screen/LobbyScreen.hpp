#ifndef LOBBY_SCREEN_HPP
#define LOBBY_SCREEN_HPP

#include "Screen/UIScreen.hpp"

namespace UI {

class LobbyScreen : public UIScreen {
   public:
    LobbyScreen();

    void Update() override;

    ScreenType GetNextScreenType() override;

    ScreenType GetScreenType() const override;
    
   private:
    // 放需要設定偵測的物件
    std::shared_ptr<UI::Button> m_PlayButton;
};

}  // namespace UI

#endif