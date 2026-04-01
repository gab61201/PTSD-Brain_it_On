#ifndef GAME_SCREEN_HPP
#define GAME_SCREEN_HPP

#include "Screen/UIScreen.hpp"
#include "Level/Level.hpp"

namespace UI {

class GameScreen : public UIScreen {
   public:
    explicit GameScreen(LevelId* levelId);

    void Update() override;

    ScreenType GetNextScreenType() override;
    
    ScreenType GetScreenType() const override;

   private:
    Level m_Level;
    std::shared_ptr<Util::GameObject> m_TargetText;
    std::shared_ptr<Util::Text> m_TimerText;
    std::shared_ptr<Util::GameObject> m_TimerObject;
};

}  // namespace UI

#endif
