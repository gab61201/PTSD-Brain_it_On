#pragma once

#include "Screen/UIScreen.hpp"

namespace UI {

class SettingsScreen : public UIScreen {
   public:
    SettingsScreen();

    ScreenType Update() override;

    ScreenType GetScreenType() const override;
};

}  // namespace UI