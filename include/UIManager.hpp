#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include "LevelManager.hpp"
#include "UIScreen.hpp"

class UIManager {
   public:
    void Update();

   private:
    UIScreenType m_CurrentUI = UIScreenType::LOBBY;
    LevelManager m_Level;
    std::unique_ptr<UIScreen> m_CurrentScreen;

    void ChangeUI(UIScreenType nextUI);

    std::unique_ptr<UIScreen> CreateScreen(UIScreenType screenType);
};

#endif
