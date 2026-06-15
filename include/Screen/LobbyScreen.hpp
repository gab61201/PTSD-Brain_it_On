#pragma once

#include "GameWorld/PhysicalWorld.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class LobbyScreen : public UIScreen {
   public:
    LobbyScreen();

    ScreenType Update() override;

    ScreenType GetScreenType() const override { return ScreenType::LOBBY; }

   private:
    std::shared_ptr<GameWorld::PhysicalWorld> m_World;
    void ResetPlayground();
};

}  // namespace UI