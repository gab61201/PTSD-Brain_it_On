#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include "Level/Level.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class MenuScreen : public UIScreen {
   public:
    explicit MenuScreen(LevelId* levelId);

    void Update() override;

    ScreenType GetNextScreenType() const override;

   private:
    LevelId *m_LevelId;
};

}  // namespace UI

#endif