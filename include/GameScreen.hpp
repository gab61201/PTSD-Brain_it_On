#ifndef GAME_SCREEN_HPP
#define GAME_SCREEN_HPP

#include "LevelManager.hpp"
#include "UIScreen.hpp"

class GameScreen : public UIScreen {
   public:
    explicit GameScreen(LevelManager& level);

    void Enter() override;

    UIScreenType Update() override;

   private:
    LevelManager& m_Level;
};

#endif