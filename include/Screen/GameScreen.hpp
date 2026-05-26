#pragma once

#include "Level/Level.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class GameScreen : public UIScreen {
   public:
    explicit GameScreen(LevelId levelId);

    ScreenType Update() override;

    ScreenType GetScreenType() const override { return ScreenType::GAME; }

    const LevelResult& GetLastResult() const { return m_Level.GetLastResult(); }

   private:
    Level m_Level;
};

}  // namespace UI
