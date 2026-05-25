#ifndef SCREEN_SETTINGS_SCREEN_HPP
#define SCREEN_SETTINGS_SCREEN_HPP

#include "Screen/UIScreen.hpp"

namespace UI {

class SettingsScreen : public UIScreen {
   public:
    SettingsScreen();

    ScreenType Update() override;

    ScreenType GetScreenType() const override;
};

}  // namespace UI

#endif  // SCREEN_SETTINGS_SCREEN_HPP