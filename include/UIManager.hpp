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

    UIState GetCurrentPhase() const { return m_CurrentPhase; }

    void Lobby();

    void Menu();

    void Game();

    void Settings();

   private:
    UIState m_CurrentPhase = UIState::LOBBY;
    std::vector<std::shared_ptr<Core::Drawable>> m_Drawables;
    std::vector<std::shared_ptr<Util::GameObject>> m_GameObjects;
};

#endif
