#ifndef SETTINGS_SCREEN_HPP
#define SETTINGS_SCREEN_HPP

#include "UIScreen.hpp"

class SettingsScreen : public UIScreen {
   public:
    void Enter() override;

    UIScreenType Update() override;
};

#endif