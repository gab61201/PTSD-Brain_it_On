#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include "Level.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class MenuScreen : public UIScreen {
   public:
    void Initialize() override;

    ScreenType Update() override;

    LevelId GetSelectedLevelType() const { return m_LevelId; }

   private:
    LevelId m_LevelId;
};

}  // namespace UI

#endif