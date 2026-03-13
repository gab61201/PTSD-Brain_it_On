#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include "LevelManager.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class MenuScreen : public UIScreen {
   public:
    explicit MenuScreen(LevelManager& level);

    void Initialize() override;

    ScreenType Update() override;

   private:
    LevelManager& m_Level;
};

}  // namespace UI

#endif