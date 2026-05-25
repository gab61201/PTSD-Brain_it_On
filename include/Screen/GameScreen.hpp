#pragma once

#include "Level/Level.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class GameScreen : public UIScreen {
   public:
    explicit GameScreen(LevelId levelId);

    ScreenType Update() override;

    ScreenType GetScreenType() const override;

    const LevelResult& GetLastResult() const { return m_Level.GetLastResult(); }
    bool IsNewRecord() const { return m_Level.IsNewRecord(); }

   private:
    Level m_Level;
};

}  // namespace UI
