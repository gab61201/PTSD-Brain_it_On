#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include "Level.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class MenuScreen : public UIScreen {
   public:
    void Initialize() override;

    ScreenType Update() override;

    LevelType GetSelectedLevelType() const { return m_LevelType; }

   private:
    LevelType m_LevelType;
};

}  // namespace UI

#endif