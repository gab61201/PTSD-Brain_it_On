#include "Screen/GameScreen.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "Level/LevelData.hpp"

namespace {

std::shared_ptr<Util::GameObject> TargetText(const std::string& text) {
    auto targetText = std::make_shared<Util::GameObject>();
    if (!text.empty()) {
        targetText->SetDrawable(std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 30, text, Util::Color::FromRGB(255, 255, 255)));
        targetText->m_Transform.translation = {0.0f, 250.0f};
    }
    return targetText;
}

std::shared_ptr<Util::GameObject> LevelNumberText(LevelId id, float x) {
    int levelNum = static_cast<int>(id) + 1;
    auto levelNumTextDrawable = std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 36, std::to_string(levelNum), Util::Color::FromRGB(255, 255, 255));
    auto levelNumberText = std::make_shared<Util::GameObject>(levelNumTextDrawable, 0.5f);
    levelNumberText->m_Transform.translation = {x + 8, 290.0f};
    return levelNumberText;
}

} // namespace

namespace UI {

GameScreen::GameScreen(LevelId* levelId) : m_Level(*levelId) {
    m_NextScreenType = ScreenType::GAME;

    auto background = UI::Element::Background("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    // ==========================================
    // 左側 UI 靜態圖示
    // ==========================================
    float leftUI_X = -520.0f;

    // 1. 關卡編號外框
    auto levelFrameImage = std::make_shared<Util::Image>("Resources/Images/level_frame.png");
    auto levelFrame = std::make_shared<Util::GameObject>(levelFrameImage, 0.1f);
    levelFrame->m_Transform.translation = {leftUI_X, 280.0f};
    levelFrame->m_Transform.scale = {0.2f, 0.2f};
    m_Renderer.AddChild(levelFrame);

    // 2. 關卡編號文字
    auto levelNumText = LevelNumberText(*levelId, leftUI_X);
    m_Renderer.AddChild(levelNumText);

    // 3. 計時器鬧鐘圖示
    auto alarmImage = std::make_shared<Util::Image>("Resources/Images/alarm.png");
    auto alarmIcon = std::make_shared<Util::GameObject>(alarmImage, 0.1f);
    alarmIcon->m_Transform.translation = {leftUI_X, 160.0f};
    alarmIcon->m_Transform.scale = {0.12f, 0.12f};
    m_Renderer.AddChild(alarmIcon);

    // 4. 計時器文字
    m_TimerText = std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 26, "0.0", Util::Color::FromRGB(255, 255, 255));
    m_TimerObject = std::make_shared<Util::GameObject>(m_TimerText, 0.5f);
    m_TimerObject->m_Transform.translation = {leftUI_X + 7, 110.0f};
    m_Renderer.AddChild(m_TimerObject);

    // 5. 提示文字
    LevelData data = GetLevelData(*levelId);
    m_TargetText = TargetText(data.targetText);
    m_Renderer.AddChild(m_TargetText);

    // 6. 返回按鈕
    auto backButton = UI::Element::CircleButton([this]{
        m_NextScreenType = ScreenType::MENU;
    }, "Resources/Images/Btn_Back.png");
    backButton->m_Transform.translation ={-560.0f, -300.0f};
    m_Buttons.push_back(backButton);
    m_Renderer.AddChild(backButton);

    // ==========================================
    // 右側 UI 靜態圖示
    // ==========================================
    auto resetButton = UI::Element::CircleButton([this]{
        m_Level.Reset();
    }, "Resources/Images/Btn_Retry.png");
    resetButton->m_Transform.translation ={560.0f, -300.0f};
    m_Buttons.push_back(resetButton);
    m_Renderer.AddChild(resetButton);
}

void GameScreen::Update() {
    for (auto button : m_Buttons) {
        button->Update();
    }

    // 更新計時器內容
    if (m_TimerText) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << m_Level.GetRemainingTime();
        m_TimerText->SetText(ss.str());
    }

    // 提示文字僅在等待狀態顯示
    if (m_TargetText) {
        m_TargetText->SetVisible(m_Level.GetState() == Level::State::WAITING);
    }

    m_Renderer.Update();
    m_Level.Update();
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_NextScreenType = ScreenType::MENU;
    }
}

ScreenType GameScreen::GetNextScreenType() {
    return m_NextScreenType;
}

ScreenType GameScreen::GetScreenType() const {
    return ScreenType::GAME;
}

}  // namespace UI