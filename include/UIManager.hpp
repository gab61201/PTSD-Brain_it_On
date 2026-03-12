#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP
#include "LevelManager.hpp"
#include "Util/GameObject.hpp"

class UIManager {
   public:
    void Update();

   private:
    enum class UIState {
        LOBBY,
        MENU,
        GAME,
        SETTINGS
    };

    // 當前顯示的 UI
    UIState m_CurrentUI = UIState::LOBBY;

    LevelManager level;

    // 當前 UI 物件暫存，切換 UI 時清除
    std::vector<std::shared_ptr<Core::Drawable>> m_Drawables;
    std::vector<std::shared_ptr<Util::GameObject>> m_GameObjects;

    void ChangeUI(UIState ui_);

    void RenderLobby();

    void RenderMenu();

    void RenderSettings();

    void Game();
};

#endif
