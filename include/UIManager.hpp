#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include "Level.hpp"
#include "Screen/UIScreen.hpp"

class UIManager {
   public:
    void Update();

   private:
    UI::ScreenType m_CurrentScreenType = UI::ScreenType::LOBBY;
    LevelId m_SelectedLevelId;

    std::unique_ptr<UI::UIScreen> m_CurrentScreen;

    void ChangeUI(UI::ScreenType nextScreenType);

    std::unique_ptr<UI::UIScreen> CreateScreen(UI::ScreenType screenType);
};

#endif
