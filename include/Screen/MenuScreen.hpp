#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include "Level.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class MenuScreen : public UIScreen {
   public:
    explicit MenuScreen(LevelId* levelId);

    ScreenType Update() override;

    ScreenType GetScreenType() const override { return ScreenType::MENU; };

   private:
    LevelId *m_LevelId;
};

}  // namespace UI

#endif