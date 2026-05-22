#ifndef RESULT_SCREEN_HPP
#define RESULT_SCREEN_HPP

#include "Level/LevelData.hpp"  
#include "Screen/UIScreen.hpp"

namespace UI {

class ResultScreen : public UIScreen {
   public:
    explicit ResultScreen(LevelResult resultData);

    ScreenType Update() override;

    ScreenType GetScreenType() const override;

   private:
    LevelResult m_ResultData;
};

}  // namespace UI

#endif
