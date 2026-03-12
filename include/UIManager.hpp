#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include "LevelManager.hpp"
#include "Screen/UIScreen.hpp"

class UIManager {
   public:
    void Update();

   private:
    UI::ScreenType m_CurrentUI = UI::ScreenType::LOBBY;
    LevelManager m_Level;
    std::unique_ptr<UI::UIScreen> m_CurrentScreen;

    void ChangeUI(UI::ScreenType nextUI);

    std::unique_ptr<UI::UIScreen> CreateScreen(UI::ScreenType screenType);
};

#endif
