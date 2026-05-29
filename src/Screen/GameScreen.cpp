#include "Screen/GameScreen.hpp"

#include "Level/LevelData.hpp"
#include "Screen/UIElement.hpp"
#include "Util/Screenshot.hpp"
#include "Constants.hpp"

namespace UI {

GameScreen::GameScreen(LevelId levelId) : UIScreen(ScreenType::GAME), m_Level(levelId) {

    auto background = UI::Element::Background(Path::Background);
    m_Renderer.AddChild(background);

    auto backButton = UI::Element::Button(Path::BtnBack, [this] {
        m_Level.Save();
        // 返回不經 ResultScreen，未成為最佳紀錄的截圖在此清掉，避免孤兒檔案
        const auto& result = m_Level.GetLastResult();
        if (!result.isNewRecord) {
            Util::Screenshot::Remove(result.screenshotFilename);
        }
        m_NextScreenType = ScreenType::MENU;
    });
    backButton->m_Transform.translation = {-560.0f, -300.0f};
    m_Buttons.push_back(backButton);
    m_Renderer.AddChild(backButton);

    // 重試按鈕
    auto resetButton = UI::Element::Button(Path::BtnRetry, [this] {
        m_Level.Reset();
    });
    resetButton->m_Transform.translation = {560.0f, -300.0f};
    m_Buttons.push_back(resetButton);
    m_Renderer.AddChild(resetButton);
}

ScreenType GameScreen::Update() {
    m_Renderer.Update();
    m_Level.Update();
    if (m_Level.GetState() == Level::State::FINISHED) {
        m_NextScreenType = ScreenType::RESULT;
    }
    for (auto button : m_Buttons) {
        button->Update();
    }
    return m_NextScreenType;
}

}  // namespace UI