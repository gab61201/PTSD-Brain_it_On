#pragma once

#include "Level/LevelData.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class ResultScreen : public UIScreen {
   public:
    explicit ResultScreen(LevelResult resultData);

    ScreenType Update() override;

    ScreenType GetScreenType() const override;

    LevelId GetNextLevelId() const { return m_NextLevelId; }

   private:
    LevelResult m_ResultData;
    LevelId m_NextLevelId;
};

}  // namespace UI
