#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include "Level/Level.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class MenuScreen : public UIScreen {
   public:
    MenuScreen();

    ScreenType Update() override;

    ScreenType GetScreenType() const override;

    LevelId GetSelectedLevelId() const;

   private:
    LevelId m_LevelId;
};

}  // namespace UI

#endif