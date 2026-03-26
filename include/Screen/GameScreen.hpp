#ifndef GAME_SCREEN_HPP
#define GAME_SCREEN_HPP

#include "Screen/UIScreen.hpp"
#include "Level/Level.hpp"

namespace UI {

class GameScreen : public UIScreen {
   public:
    explicit GameScreen(LevelId* levelId);

    void Update() override;

    ScreenType GetNextScreenType() const override;
    
    ScreenType GetScreenType() const override;

   private:
    Level m_Level;
};

}  // namespace UI

#endif
