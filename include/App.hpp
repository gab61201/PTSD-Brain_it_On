#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"  // IWYU pragma: export

#include "Screen/UIScreen.hpp"
#include "Screen/LobbyScreen.hpp"
#include "Screen/MenuScreen.hpp"
#include "Screen/GameScreen.hpp"
#include "Screen/SettingsScreen.hpp"

class App {
   public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End();  // NOLINT(readability-convert-member-functions-to-static)

   private:
    State m_CurrentState = State::START;
    
    // UI
    UI::ScreenType m_ScreenType;
    std::unique_ptr<UI::UIScreen> m_Screen;
    LevelId m_SelectedLevelId;
};

#endif
