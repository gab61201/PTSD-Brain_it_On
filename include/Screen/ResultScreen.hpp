#ifndef RESULT_SCREEN_HPP
#define RESULT_SCREEN_HPP

#include "Level/Level.hpp"
#include "Screen/UIScreen.hpp"

namespace UI {

class ResultScreen : public UIScreen {
   public:
    ResultScreen(LevelId* levelId, const LevelResultData& resultData);

    ScreenType Update() override;

    ScreenType GetScreenType() const override;

   private:
    LevelResultData m_ResultData;
};

}  // namespace UI

#endif
