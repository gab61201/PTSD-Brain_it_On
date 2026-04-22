#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include "Level/Level.hpp"
#include "Progress/ProgressStore.hpp"
#include "Screen/UIScreen.hpp"

class ProgressStore;

namespace UI {

class MenuScreen : public UIScreen {
   public:
    explicit MenuScreen(LevelId* levelId, ProgressStore* progressStore);

    void Update() override;

    ScreenType GetNextScreenType() override;

    ScreenType GetScreenType() const override;

   private:
    LevelId* m_LevelId;
    ProgressStore* m_ProgressStore;
    ScreenType m_NextScreenType = ScreenType::MENU;
    std::shared_ptr<Util::Text> m_PromptText;
    std::shared_ptr<Util::GameObject> m_PromptObject;
    int m_PromptFramesRemaining = 0;
};

}  // namespace UI

#endif