#ifndef APP_HPP
#define APP_HPP

#include "Progress/ProgressStore.hpp"
#include "Screen/GameScreen.hpp"
#include "Screen/LobbyScreen.hpp"
#include "Screen/MenuScreen.hpp"
#include "Screen/SettingsScreen.hpp"
#include "Screen/UIScreen.hpp"
#include "pch.hpp"  // IWYU pragma: export

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
    ProgressStore m_ProgressStore;
};

#endif
