#ifndef SCREEN_MENU_SCREEN_HPP
#define SCREEN_MENU_SCREEN_HPP

#include "Level/LevelData.hpp"
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

#endif  // SCREEN_MENU_SCREEN_HPP