#include "Level/LevelHUD.hpp"

#include <glm/vec2.hpp>
#include <iomanip>
#include <sstream>

#include "Constants.hpp"
#include "Screen/UIElement.hpp"
#include "Util/Color.hpp"
#include "Util/Text.hpp"

namespace {

constexpr float LEFT_UI_X = -520.0f;

const Util::Color WHITE = Util::Color::FromRGB(255, 255, 255);

void SetText(std::shared_ptr<Util::GameObject> object,
             const std::string& text, int size) {
    if (object) {
        object->SetDrawable(std::make_shared<Util::Text>(
            "Resources/Fonts/Inter.ttf", size, text, WHITE));
    }
}

}  // namespace

LevelHUD::LevelHUD(LevelId levelId, const std::string& targetText, int strokeLimit) {
    // 1. 關卡編號外框
    auto levelFrame = UI::Element::Image(
        Path::LevelFrame, {LEFT_UI_X, 280.0f}, {0.2f, 0.2f}, 0.1f);
    m_Renderer.AddChild(levelFrame);

    // 2. 關卡編號文字
    auto levelNumText = UI::Element::Text(
        std::to_string(static_cast<int>(levelId) + 1), 36, {LEFT_UI_X + 8, 290.0f}, WHITE, 0.5f);
    m_Renderer.AddChild(levelNumText);

    // 3. 計時器鬧鐘圖示
    auto alarmIcon = UI::Element::Image(
        Path::Alarm, {LEFT_UI_X, 160.0f}, {0.12f, 0.12f}, 0.1f);
    m_Renderer.AddChild(alarmIcon);

    // 4. 計時器文字
    m_TimerObject = UI::Element::Text(
        "0.0", 26, {LEFT_UI_X + 7, 110.0f}, WHITE, 0.5f);
    m_Renderer.AddChild(m_TimerObject);

    // 5. 提示文字
    m_TargetTextObject = UI::Element::Text(targetText, 30, {0.0f, 250.0f}, WHITE, 0.5f);
    ;
    m_Renderer.AddChild(m_TargetTextObject);

    // 6. 限制筆劃次數圖示
    auto strokeLimitIcon = UI::Element::Image(
        Path::StrokeLimit, {LEFT_UI_X - 5, 50.0f}, {0.12f, 0.12f}, 0.1f);
    m_Renderer.AddChild(strokeLimitIcon);

    // 7. 限制筆劃次數文字
    m_StrokeLimitObject = UI::Element::Text(
        std::to_string(strokeLimit) + "/" + std::to_string(strokeLimit),
        24, {LEFT_UI_X + 8.0f, 7.0f}, WHITE, 0.5f);
    m_Renderer.AddChild(m_StrokeLimitObject);

    // 8. 接觸倒數計時器（初始為隱藏）
    m_ContactTimerObject = UI::Element::Text(
        " ", 256, {0.0f, 0.0f}, WHITE, 0.5f);
    m_ContactTimerObject->SetVisible(false);
    m_Renderer.AddChild(m_ContactTimerObject);
}

void LevelHUD::UpdateTimer(float remainingTime) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << remainingTime;
    SetText(m_TimerObject, ss.str(), 26);
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
    m_TargetTextObject = UI::Element::Text(targetText, 30, {0.0f, 250.0f}, WHITE, 0.5f);
    ;
    m_Renderer.AddChild(m_TargetTextObject);

    // 重置計時器
    SetText(m_TimerObject, "0.0", 26);

    UpdateStrokeLimit(strokeLimit, strokeLimit);
}

void LevelHUD::UpdateStrokeLimit(int remainingStroke, int totalStrokeLimit) {
    SetText(m_StrokeLimitObject,
            std::to_string(remainingStroke) + "/" + std::to_string(totalStrokeLimit),
            24);
}

void LevelHUD::UpdateContactTimer(int contactCountDown) {
    if (contactCountDown > 0) {
        m_ContactTimerObject->SetVisible(true);
        SetText(m_ContactTimerObject, std::to_string(contactCountDown), 256);
    } else {
        m_ContactTimerObject->SetVisible(false);
    }
}

void LevelHUD::Update() {
    m_Renderer.Update();
}
