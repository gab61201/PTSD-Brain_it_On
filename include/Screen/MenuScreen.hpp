#pragma once

#include "Level/LevelData.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class MenuScreen : public UIScreen {
   public:
    MenuScreen();

    ScreenType Update() override;

    ScreenType GetScreenType() const override { return ScreenType::MENU; }

    LevelId GetSelectedLevelId() const { return m_LevelId; }

   private:
    LevelId m_LevelId;
};

}  // namespace UI