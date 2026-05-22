#include "Screen/ResultScreen.hpp"

#include <iomanip>
#include <sstream>

#include "Level/LevelData.hpp"
#include "Screen/UIElement.hpp"
#include "Util/Color.hpp"
#include "Util/Input.hpp"
#include "Util/ProgressStore.hpp"

namespace {

std::string FormatSeconds(float seconds) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << seconds << "s";
    return ss.str();
}

}  // namespace

namespace UI {

ResultScreen::ResultScreen(LevelId* levelId)
    : m_ResultData(Util::ProgressStore::s_LastPlayedLevelData) {
    (void)levelId;
    const bool withinTimeLimit = (m_ResultData.solvedTime <= m_ResultData.goalTime);
    const bool withinStrokeLimit = (m_ResultData.usedStroke <= m_ResultData.goalStroke);
    const Util::Color solvedTimeColor = withinTimeLimit
                                            ? Util::Color::FromRGB(56, 209, 83)
                                            : Util::Color::FromRGB(255, 77, 77);
    const Util::Color solvedStrokeColor = withinStrokeLimit
                                              ? Util::Color::FromRGB(56, 209, 83)
                                              : Util::Color::FromRGB(255, 77, 77);

    m_NextScreenType = ScreenType::RESULT;

    auto background = UI::Element::Background("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    auto panel = UI::Element::Image(
        "Resources/Images/BasicShapes/blue_square.png",
        {0.0f, -48.0f},
        {1.72f, 0.98f},
        0.1f);
    m_Renderer.AddChild(panel);

    auto header = UI::Element::Image(
        "Resources/Images/BasicShapes/orange_square.png",
        {0.0f, 156.0f},
        {1.72f, 0.13f},
        0.2f);
    m_Renderer.AddChild(header);

    const std::string title = m_ResultData.passed ? "Nice one!" : "Try again!";
    m_Renderer.AddChild(UI::Element::Text(
        title, 64, {0.0f, 262.0f}, Util::Color::FromRGB(255, 255, 255), 1.5f));

    m_Renderer.AddChild(UI::Element::Text(
        "Level #" + std::to_string(static_cast<int>(m_ResultData.levelId) + 1),
        48,
        {0.0f, 156.0f},
        Util::Color::FromRGB(74, 74, 74),
        1.5f));

    const glm::vec2 colLeft = {-180.0f, 0.0f};
    const glm::vec2 colMid = {0.0f, 0.0f};
    const glm::vec2 colRight = {180.0f, 0.0f};

    const glm::vec2 starLeft = {-150.0f, 0.0f};
    const glm::vec2 starMid = {0.0f, 0.0f};
    const glm::vec2 starRight = {150.0f, 0.0f};

    m_Renderer.AddChild(UI::Element::Image(
        m_ResultData.passed ? "Resources/Images/star_bright.png"
                            : "Resources/Images/star_dark.png",
        {starLeft.x, 80.0f}, {0.50, 0.50}, 1.3f));
    m_Renderer.AddChild(UI::Element::Image(
        withinTimeLimit ? "Resources/Images/star_bright.png"
                        : "Resources/Images/star_dark.png",
        {starMid.x, 80.0f}, {0.50, 0.50}, 1.3f));
    m_Renderer.AddChild(UI::Element::Image(
        withinStrokeLimit ? "Resources/Images/star_bright.png"
                          : "Resources/Images/star_dark.png",
        {starRight.x, 80.0f}, {0.50, 0.50}, 1.3f));

    m_Renderer.AddChild(UI::Element::Text(
        "\u2713", 40, {colLeft.x, 0.0f}, Util::Color::FromRGB(56, 209, 83), 1.4f));
    m_Renderer.AddChild(UI::Element::Image(
        "Resources/Images/alarm.png", {colMid.x, -2.0f}, {0.068f, 0.068f}, 1.4f));
    m_Renderer.AddChild(UI::Element::Image(
        "Resources/Images/stroke_limit.png", {colRight.x, -2.0f}, {0.068f, 0.068f}, 1.4f));

    m_Renderer.AddChild(UI::Element::Text(
        "Goal:", 48, {-215.0f, -70.0f}, Util::Color::FromRGB(245, 245, 245), 1.4f));
    m_Renderer.AddChild(UI::Element::Text(
        FormatSeconds(m_ResultData.goalTime),
        48,
        {colMid.x, -70.0f},
        Util::Color::FromRGB(245, 245, 245),
        1.4f));
    m_Renderer.AddChild(UI::Element::Text(
        std::to_string(m_ResultData.goalStroke),
        48,
        {colRight.x, -70.0f},
        Util::Color::FromRGB(245, 245, 245),
        1.4f));

    auto solvedStrip = UI::Element::Image(
        "Resources/Images/BasicShapes/white_square.png",
        {0.0f, -152.0f},
        {1.72f, 0.16f},
        1.1f);
    m_Renderer.AddChild(solvedStrip);

    m_Renderer.AddChild(UI::Element::Text(
        "Solved",
        48,
        {-215.0f, -152.0f},
        Util::Color::FromRGB(11, 49, 80),
        1.6f));
    m_Renderer.AddChild(UI::Element::Text(
        FormatSeconds(m_ResultData.solvedTime),
        48,
        {colMid.x, -152.0f},
        solvedTimeColor,
        1.6f));
    m_Renderer.AddChild(UI::Element::Text(
        std::to_string(m_ResultData.usedStroke),
        48,
        {colRight.x, -152.0f},
        solvedStrokeColor,
        1.6f));

    auto selectButton = UI::Element::CircleButton([this]() {
        m_NextScreenType = ScreenType::MENU;
    },
                                                  "Resources/Images/Btn_Back.png");
    selectButton->m_Transform.translation = {-220.0f, -322.0f};
    m_Buttons.push_back(selectButton);
    m_Renderer.AddChild(selectButton);

    auto retryButton = UI::Element::CircleButton([this]() {
        m_NextScreenType = ScreenType::GAME;
    },
                                                 "Resources/Images/Btn_Retry.png");
    retryButton->m_Transform.translation = {0.0f, -322.0f};
    m_Buttons.push_back(retryButton);
    m_Renderer.AddChild(retryButton);

    auto nextButton = UI::Element::CircleButton([this]() {
        m_NextScreenType = ScreenType::MENU;
    },
                                                "Resources/Images/Btn_Next.png");
    nextButton->m_Transform.translation = {220.0f, -322.0f};

    m_Buttons.push_back(nextButton);
    m_Renderer.AddChild(nextButton);
}

ScreenType ResultScreen::Update() {
    m_Renderer.Update();
    for (const auto& button : m_Buttons) {
        button->Update();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_NextScreenType = ScreenType::MENU;
    }
    return m_NextScreenType;
}

ScreenType ResultScreen::GetScreenType() const {
    return ScreenType::RESULT;
}

}  // namespace UI
