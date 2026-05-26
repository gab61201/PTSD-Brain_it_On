#pragma once

#include "Level/LevelData.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class ResultScreen : public UIScreen {
   public:
    explicit ResultScreen(LevelResult resultData);

    ~ResultScreen();

    ScreenType Update() override;

    ScreenType GetScreenType() const override { return ScreenType::RESULT; }

    LevelId GetNextLevelId() const { return m_NextLevelId; }

   private:
    LevelResult m_ResultData;
    LevelId m_NextLevelId;
};

}  // namespace UI
