#ifndef SETTINGS_SCREEN_HPP
#define SETTINGS_SCREEN_HPP

#include "Screen/UIScreen.hpp"

namespace UI {

class SettingsScreen : public UIScreen {
   public:
    SettingsScreen();

    ScreenType Update() override;
};

}  // namespace UI

#endif