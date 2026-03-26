#ifndef SETTINGS_SCREEN_HPP
#define SETTINGS_SCREEN_HPP

#include "Screen/UIScreen.hpp"

namespace UI {

class SettingsScreen : public UIScreen {
   public:
    SettingsScreen();

    void Update() override;
    
    ScreenType GetNextScreenType() const override;

    ScreenType GetScreenType() const override;
};

}  // namespace UI

#endif