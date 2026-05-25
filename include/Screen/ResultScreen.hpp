#ifndef SCREEN_RESULT_SCREEN_HPP
#define SCREEN_RESULT_SCREEN_HPP

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

#endif  // SCREEN_RESULT_SCREEN_HPP
