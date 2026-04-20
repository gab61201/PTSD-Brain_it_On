#include "Level/LevelHUD.hpp"

#include <iomanip>
#include <sstream>

#include "Util/Color.hpp"
#include "Util/Image.hpp"

namespace {

constexpr float LEFT_UI_X = -520.0f;

std::shared_ptr<Util::GameObject> CreateTargetText(
    const std::string& text) {
    auto targetText = std::make_shared<Util::GameObject>();
    if (!text.empty()) {
        targetText->SetDrawable(std::make_shared<Util::Text>(
            "PTSD/assets/fonts/Inter.ttf", 30, text,
            Util::Color::FromRGB(255, 255, 255)));
        targetText->m_Transform.translation = {0.0f, 250.0f};
    }
    return targetText;
}

std::shared_ptr<Util::GameObject> CreateLevelNumberText(LevelId id,
                                                         float x) {
    int levelNum = static_cast<int>(id) + 1;
    auto drawable = std::make_shared<Util::Text>(
        "PTSD/assets/fonts/Inter.ttf", 36, std::to_string(levelNum),
        Util::Color::FromRGB(255, 255, 255));
    auto obj = std::make_shared<Util::GameObject>(drawable, 0.5f);
    obj->m_Transform.translation = {x + 8, 290.0f};
    return obj;
}

}  // namespace

LevelHUD::LevelHUD(LevelId levelId, const std::string& targetText, int strokeLimit) {
    // 1. 關卡編號外框
    auto levelFrameImage =
        std::make_shared<Util::Image>("Resources/Images/level_frame.png");
    auto levelFrame =
        std::make_shared<Util::GameObject>(levelFrameImage, 0.1f);
    levelFrame->m_Transform.translation = {LEFT_UI_X, 280.0f};
    levelFrame->m_Transform.scale = {0.2f, 0.2f};
    m_Renderer.AddChild(levelFrame);

    // 2. 關卡編號文字
    auto levelNumText = CreateLevelNumberText(levelId, LEFT_UI_X);
    m_Renderer.AddChild(levelNumText);

    // 3. 計時器鬧鐘圖示
    auto alarmImage =
        std::make_shared<Util::Image>("Resources/Images/alarm.png");
    auto alarmIcon =
        std::make_shared<Util::GameObject>(alarmImage, 0.1f);
    alarmIcon->m_Transform.translation = {LEFT_UI_X, 160.0f};
    alarmIcon->m_Transform.scale = {0.12f, 0.12f};
    m_Renderer.AddChild(alarmIcon);

    // 4. 計時器文字
    m_TimerText = std::make_shared<Util::Text>(
        "PTSD/assets/fonts/Inter.ttf", 26, "0.0",
        Util::Color::FromRGB(255, 255, 255));
    m_TimerObject =
        std::make_shared<Util::GameObject>(m_TimerText, 0.5f);
    m_TimerObject->m_Transform.translation = {LEFT_UI_X + 7, 110.0f};
    m_Renderer.AddChild(m_TimerObject);

    // 5. 提示文字
    m_TargetTextObject = CreateTargetText(targetText);
    m_Renderer.AddChild(m_TargetTextObject);

    // 6. 限制筆劃次數圖示
    auto strokeLimitImage =
        std::make_shared<Util::Image>("Resources/Images/stroke_limit.png");
    auto strokeLimitIcon =
        std::make_shared<Util::GameObject>(strokeLimitImage, 0.1f);
    strokeLimitIcon->m_Transform.translation = {LEFT_UI_X - 5, 50.0f};
    strokeLimitIcon->m_Transform.scale = {0.12f, 0.12f};
    m_Renderer.AddChild(strokeLimitIcon);

    // 7. 限制筆劃次數文字
    m_StrokeLimitText = std::make_shared<Util::Text>(
        "PTSD/assets/fonts/Inter.ttf", 24,
        std::to_string(strokeLimit) + "/" + std::to_string(strokeLimit),
        Util::Color::FromRGB(255, 255, 255));
    m_StrokeLimitObject = std::make_shared<Util::GameObject>(m_StrokeLimitText, 0.5f);
    m_StrokeLimitObject->m_Transform.translation = {LEFT_UI_X + 8.0f, 7.0f};
    m_Renderer.AddChild(m_StrokeLimitObject);

    // 8. 接觸倒數計時器（初始為隱藏）
    m_ContactTimerText = std::make_shared<Util::Text>(
        "PTSD/assets/fonts/Inter.ttf", 256, " ",
        Util::Color::FromRGB(255, 255, 255));
    m_ContactTimerObject =
        std::make_shared<Util::GameObject>(m_ContactTimerText, 0.5f);
    m_ContactTimerObject->SetVisible(false);  // 初始隱藏
    m_Renderer.AddChild(m_ContactTimerObject);
}

void LevelHUD::UpdateTimer(float remainingTime) {
    if (m_TimerText) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << remainingTime;
        m_TimerText->SetText(ss.str());
    }
}

void LevelHUD::HideTarget() {
    if (m_TargetTextObject) {
        m_TargetTextObject->SetVisible(false);
    }
}

void LevelHUD::Reset(const std::string& targetText, int strokeLimit) {
    // 重建提示文字
    if (m_TargetTextObject) {
        m_Renderer.RemoveChild(m_TargetTextObject);
    }
    m_TargetTextObject = CreateTargetText(targetText);
    m_Renderer.AddChild(m_TargetTextObject);

    // 重置計時器
    if (m_TimerText) {
        m_TimerText->SetText("0.0");
    }

    UpdateStrokeLimit(strokeLimit, strokeLimit);
}

void LevelHUD::UpdateStrokeLimit(int remainingStroke, int totalStrokeLimit) {
    if (m_StrokeLimitText) {
        m_StrokeLimitText->SetText(std::to_string(remainingStroke) + "/" + std::to_string(totalStrokeLimit));
    }
}

void LevelHUD::UpdateContactTimer(int contactCountDown) {
    if (m_ContactTimerText) {
        m_ContactTimerObject->SetVisible(true);  // 顯示接觸倒數計時器
        m_ContactTimerText->SetText(std::to_string(contactCountDown));
    }
}

void LevelHUD::Update() {
    m_Renderer.Update();
    m_ContactTimerObject->SetVisible(false);  // 每幀更新後隱藏接觸倒數計時器，直到下一次需要顯示
}
