#ifndef GAME_SCREEN_HPP
#define GAME_SCREEN_HPP

#include "LevelManager.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class GameScreen : public UIScreen {
   public:
    explicit GameScreen(LevelManager &level);

    void Enter() override;

    ScreenType Update() override;

   private:
    LevelManager &m_Level;
};

} // namespace UI

#endif