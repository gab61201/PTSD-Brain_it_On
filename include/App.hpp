#pragma once

#include "Level/LevelData.hpp"
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
    std::unique_ptr<UI::UIScreen> m_Screen;
    LevelId m_SelectedLevelId;
    LevelResult m_LastResult;
    bool m_IsNewRecord = false;
    std::string m_LastScreenshotFilename;
};
