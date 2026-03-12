#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include "LevelManager.hpp"
#include "UIScreen.hpp"

class MenuScreen : public UIScreen {
   public:
    explicit MenuScreen(LevelManager& level);

    void Enter() override;

    UIScreenType Update() override;

   private:
    LevelManager& m_Level;
};

#endif