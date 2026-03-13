#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include "Level.hpp"
#include "Screen/UIScreen.hpp"

class UIManager {
   public:
    void Update();

   private:
    UI::ScreenType m_CurrentUI = UI::ScreenType::LOBBY;
    LevelType m_SelectedLevelType;

    std::unique_ptr<UI::UIScreen> m_CurrentScreen;

    void ChangeUI(UI::ScreenType nextUI);

    std::unique_ptr<UI::UIScreen> CreateScreen(UI::ScreenType screenType);
};

#endif
