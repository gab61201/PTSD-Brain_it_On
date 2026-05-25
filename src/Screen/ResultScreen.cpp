#include "Screen/ResultScreen.hpp"

#include <iomanip>
#include <sstream>

#include "Level/LevelData.hpp"
#include "Screen/UIElement.hpp"
#include "Util/Color.hpp"
#include "Util/Input.hpp"
#include "Constants.hpp"

namespace {

std::string FormatSeconds(float seconds) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << seconds << "s";
    return ss.str();
}

}  // namespace

namespace UI {

ResultScreen::ResultScreen(LevelResult resultData)
    : m_ResultData(std::move(resultData)) {
    int nextLevel = static_cast<int>(m_ResultData.levelId) + 1;
    m_NextLevelId = (nextLevel <= static_cast<int>(LevelId::LEVEL_5))
                        ? static_cast<LevelId>(nextLevel)
                        : m_ResultData.levelId;
    const bool withinTimeLimit = (m_ResultData.solvedTime <= m_ResultData.goalTime);
    const bool withinStrokeLimit = (m_ResultData.usedStroke <= m_ResultData.goalStroke);
    const Util::Color solvedTimeColor = withinTimeLimit
                                            ? Util::Color::FromRGB(56, 209, 83)
                                            : Util::Color::FromRGB(255, 77, 77);
    const Util::Color solvedStrokeColor = withinStrokeLimit
                                              ? Util::Color::FromRGB(56, 209, 83)
                                              : Util::Color::FromRGB(255, 77, 77);

    m_NextScreenType = ScreenType::RESULT;

    auto background = UI::Element::Background(Path::Background);
    m_Renderer.AddChild(background);

    auto panel = UI::Element::Image(
        Path::BlueSquare,
        {0.0f, -48.0f},
        {1.72f, 0.98f},
        Layer::UIBackground);
    m_Renderer.AddChild(panel);

    auto header = UI::Element::Image(
        Path::OrangeSquare,
        {0.0f, 156.0f},
        {1.72f, 0.13f},
        Layer::UIBackground);
    m_Renderer.AddChild(header);

    const std::string title = m_ResultData.passed ? "Nice one!" : "Try again!";
    m_Renderer.AddChild(UI::Element::Text(
        title, 64, {0.0f, 262.0f}, Util::Color::FromRGB(255, 255, 255), Layer::UIElement));

    m_Renderer.AddChild(UI::Element::Text(
        "Level #" + std::to_string(static_cast<int>(m_ResultData.levelId) + 1),
        48,
        {0.0f, 156.0f},
        Util::Color::FromRGB(74, 74, 74),
        Layer::UIElement));

    const float statsCenterX = -180.0f;
    const glm::vec2 colLeft = {statsCenterX - 140.0f, 0.0f};
    const glm::vec2 colMid = {statsCenterX, 0.0f};
    const glm::vec2 colRight = {statsCenterX + 140.0f, 0.0f};

    const glm::vec2 starLeft = {colLeft.x, 0.0f};
    const glm::vec2 starMid = {colMid.x, 0.0f};
    const glm::vec2 starRight = {colRight.x, 0.0f};

    m_Renderer.AddChild(UI::Element::Image(
        m_ResultData.passed ? Path::StarBright
                            : Path::StarDark,
        {starLeft.x, 80.0f}, {0.50, 0.50}, Layer::UIElement));
    m_Renderer.AddChild(UI::Element::Image(
        withinTimeLimit ? Path::StarBright
                        : Path::StarDark,
        {starMid.x, 80.0f}, {0.50, 0.50}, Layer::UIElement));
    m_Renderer.AddChild(UI::Element::Image(
        withinStrokeLimit ? Path::StarBright
                          : Path::StarDark,
        {starRight.x, 80.0f}, {0.50, 0.50}, Layer::UIElement));

    m_Renderer.AddChild(UI::Element::Text(
        "\u2713", 40, {colLeft.x, 0.0f}, Util::Color::FromRGB(56, 209, 83), Layer::UIElement));
    m_Renderer.AddChild(UI::Element::Image(
        Path::Alarm, {colMid.x, -2.0f}, {0.068f, 0.068f}, Layer::UIElement));
    m_Renderer.AddChild(UI::Element::Image(
        Path::StrokeLimit, {colRight.x, -2.0f}, {0.068f, 0.068f}, Layer::UIElement));

    m_Renderer.AddChild(UI::Element::Text(
        "Goal:", 48, {colLeft.x - 40.0f, -70.0f}, Util::Color::FromRGB(245, 245, 245), Layer::UIElement));
    m_Renderer.AddChild(UI::Element::Text(
        FormatSeconds(m_ResultData.goalTime),
        48,
        {colMid.x, -70.0f},
        Util::Color::FromRGB(245, 245, 245),
        Layer::UIElement));
    m_Renderer.AddChild(UI::Element::Text(
        std::to_string(m_ResultData.goalStroke),
        48,
        {colRight.x, -70.0f},
        Util::Color::FromRGB(245, 245, 245),
        Layer::UIElement));

    auto solvedStrip = UI::Element::Image(
        Path::WhiteSquare,
        {statsCenterX, -152.0f},
        {0.95f, 0.16f},
        Layer::UIOutline);
    m_Renderer.AddChild(solvedStrip);

    m_Renderer.AddChild(UI::Element::Text(
        "Solved",
        48,
        {colLeft.x - 40.0f, -152.0f},
        Util::Color::FromRGB(11, 49, 80),
        Layer::UIElement));
    m_Renderer.AddChild(UI::Element::Text(
        FormatSeconds(m_ResultData.solvedTime),
        48,
        {colMid.x, -152.0f},
        solvedTimeColor,
        Layer::UIElement));
    m_Renderer.AddChild(UI::Element::Text(
        std::to_string(m_ResultData.usedStroke),
        48,
        {colRight.x, -152.0f},
        solvedStrokeColor,
        Layer::UIElement));

    auto levelFrame = UI::Element::Image(
        Path::LevelFrame,
        {230.0f, -30.0f},
        {0.38f, 0.38f},
        Layer::UIOutline);
    m_Renderer.AddChild(levelFrame);

    if (!m_ResultData.screenshotFilename.empty()) {
        auto screenshot = UI::Element::Image(
            "Resources/Save/Screenshots/" + m_ResultData.screenshotFilename,
            {230.0f, 4.0f},
            {0.32f, 0.32f},
            Layer::UIElement);
        m_Renderer.AddChild(screenshot);
    }

    auto selectButton = UI::Element::Button(Path::BtnBack, [this]() {
        m_NextScreenType = ScreenType::MENU;
    });
    selectButton->m_Transform.translation = {-220.0f, -322.0f};
    m_Buttons.push_back(selectButton);
    m_Renderer.AddChild(selectButton);

    auto retryButton = UI::Element::Button(Path::BtnRetry, [this]() {
        m_NextScreenType = ScreenType::GAME;
    });
    retryButton->m_Transform.translation = {0.0f, -322.0f};
    m_Buttons.push_back(retryButton);
    m_Renderer.AddChild(retryButton);

    auto nextButton = UI::Element::Button(Path::BtnNext, [this]() {
        m_NextScreenType = (m_NextLevelId != m_ResultData.levelId)
                               ? ScreenType::GAME
                               : ScreenType::MENU;
    });
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
