#ifndef LOBBY_SCREEN_HPP
#define LOBBY_SCREEN_HPP

#include "Screen/UIScreen.hpp"

namespace UI {

class LobbyScreen : public UIScreen {
   public:
    void Enter() override;

    ScreenType Update() override;
};

} // namespace UI

#endif