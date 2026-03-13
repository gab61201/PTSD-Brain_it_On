#ifndef GAME_SCREEN_HPP
#define GAME_SCREEN_HPP

#include "Level.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class GameScreen : public UIScreen {
   public:
    explicit GameScreen(LevelId levelId);

    void Initialize() override;

    ScreenType Update() override;

   private:
    Level m_Level;
    LevelId m_LevelId;
};

}  // namespace UI

#endif
