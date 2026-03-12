#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP
#include "Util/GameObject.hpp"

class UIManager {
   public:
    enum class UIState {
        LOBBY,
        MENU,
        GAME,
        SETTINGS
    };

    UIState GetCurrentUI() const { return m_CurrentUI; }

    void Update();
    
    void RenderLobby();

    void RenderMenu();

    void Game();

    void RenderSettings();

   private:
    // 當前顯示的 UI
    UIState m_CurrentUI = UIState::LOBBY;
    // 當前 UI 元件快取，切換 UI 時清除
    std::vector<std::shared_ptr<Core::Drawable>> m_Drawables;
    std::vector<std::shared_ptr<Util::GameObject>> m_GameObjects;
};

#endif
