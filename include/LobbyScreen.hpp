#ifndef LOBBY_SCREEN_HPP
#define LOBBY_SCREEN_HPP

#include "UIScreen.hpp"

class LobbyScreen : public UIScreen {
   public:
    void Enter() override;

    UIScreenType Update() override;
};

#endif