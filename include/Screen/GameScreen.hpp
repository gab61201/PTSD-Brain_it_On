#ifndef GAME_SCREEN_HPP
#define GAME_SCREEN_HPP

#include "Screen/UIScreen.hpp"
#include "Level/Level.hpp"

namespace UI {

class GameScreen : public UIScreen {
   public:
    explicit GameScreen(LevelId* levelId);

    ScreenType Update() override;

    ScreenType GetScreenType() const override { return ScreenType::GAME; };

   private:
    Level m_Level;
};

}  // namespace UI

#endif
