#include "Screen/ResultScreen.hpp"

#include <iomanip>
#include <sstream>

#include "Level/LevelData.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"

namespace {

std::shared_ptr<Util::GameObject> CreateTextObject(
    const std::string& text,
    int size,
    const glm::vec2& position,
    const Util::Color& color,
    float z = 1.0f) {
    auto drawable =
        std::make_shared<Util::Text>("Resources/Fonts/Inter.ttf", size, text, color);
    auto object = std::make_shared<Util::GameObject>(drawable, z);
    object->m_Transform.translation = position;
    return object;
}

std::shared_ptr<Util::GameObject> CreateImageObject(
    const std::string& path,
    const glm::vec2& position,
    const glm::vec2& scale,
    float z = 1.0f) {
    auto drawable = std::make_shared<Util::Image>(path);
    auto object = std::make_shared<Util::GameObject>(drawable, z);
    object->m_Transform.translation = position;
    object->m_Transform.scale = scale;
    return object;
}

std::string FormatSeconds(float seconds) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << seconds << "s";
    return ss.str();
}

int ToLevelNumber(LevelId id) {
    return static_cast<int>(id) + 1;
}

}  // namespace

namespace UI {

ResultScreen::ResultScreen(LevelId* levelId, const LevelResultData& resultData)
    : m_ResultData(resultData) {
    (void)levelId;
    const bool withinTimeLimit = IsWithinTimeLimit(m_ResultData);
    const bool withinStrokeLimit = IsWithinStrokeLimit(m_ResultData);
    const Util::Color solvedTimeColor = withinTimeLimit
                                            ? Util::Color::FromRGB(56, 209, 83)
                                            : Util::Color::FromRGB(255, 77, 77);
    const Util::Color solvedStrokeColor = withinStrokeLimit
                                              ? Util::Color::FromRGB(56, 209, 83)
                                              : Util::Color::FromRGB(255, 77, 77);

    m_NextScreenType = ScreenType::RESULT;

    auto background = UI::Element::Background("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    auto panel = CreateImageObject(
        "Resources/Images/BasicShapes/blue_square.png",
        {0.0f, -48.0f},
        {1.72f, 0.98f},
        0.1f);
    m_Renderer.AddChild(panel);

    auto header = CreateImageObject(
        "Resources/Images/BasicShapes/orange_square.png",
        {0.0f, 156.0f},
        {1.72f, 0.13f},
        0.2f);
    m_Renderer.AddChild(header);

    const std::string title = m_ResultData.passed ? "Nice one!" : "Try again!";
    m_Renderer.AddChild(CreateTextObject(
        title, 64, {0.0f, 262.0f}, Util::Color::FromRGB(255, 255, 255), 1.5f));

    m_Renderer.AddChild(CreateTextObject(
        "Level #" + std::to_string(ToLevelNumber(m_ResultData.levelId)),
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

    m_Renderer.AddChild(CreateImageObject(
        m_ResultData.passed ? "Resources/Images/star_bright.png"
                            : "Resources/Images/star_dark.png",
        {starLeft.x, 80.0f}, {0.50, 0.50}, 1.3f));
    m_Renderer.AddChild(CreateImageObject(
        withinTimeLimit ? "Resources/Images/star_bright.png"
                        : "Resources/Images/star_dark.png",
        {starMid.x, 80.0f}, {0.50, 0.50}, 1.3f));
    m_Renderer.AddChild(CreateImageObject(
        withinStrokeLimit ? "Resources/Images/star_bright.png"
                          : "Resources/Images/star_dark.png",
        {starRight.x, 80.0f}, {0.50, 0.50}, 1.3f));

    m_Renderer.AddChild(CreateTextObject(
        "✓", 40, {colLeft.x, 0.0f}, Util::Color::FromRGB(56, 209, 83), 1.4f));
    m_Renderer.AddChild(CreateImageObject(
        "Resources/Images/alarm.png", {colMid.x, -2.0f}, {0.068f, 0.068f}, 1.4f));
    m_Renderer.AddChild(CreateImageObject(
        "Resources/Images/stroke_limit.png", {colRight.x, -2.0f}, {0.068f, 0.068f}, 1.4f));

    m_Renderer.AddChild(CreateTextObject(
        "Goal:", 48, {-215.0f, -70.0f}, Util::Color::FromRGB(245, 245, 245), 1.4f));
    m_Renderer.AddChild(CreateTextObject(
        FormatSeconds(m_ResultData.goalTime),
        48,
        {colMid.x, -70.0f},
        Util::Color::FromRGB(245, 245, 245),
        1.4f));
    m_Renderer.AddChild(CreateTextObject(
        std::to_string(m_ResultData.goalStroke),
        48,
        {colRight.x, -70.0f},
        Util::Color::FromRGB(245, 245, 245),
        1.4f));

    auto solvedStrip = CreateImageObject(
        "Resources/Images/BasicShapes/white_square.png",
        {0.0f, -152.0f},
        {1.72f, 0.16f},
        1.1f);
    m_Renderer.AddChild(solvedStrip);

    m_Renderer.AddChild(CreateTextObject(
        "Solved",
        48,
        {-215.0f, -152.0f},
        Util::Color::FromRGB(11, 49, 80),
        1.6f));
    m_Renderer.AddChild(CreateTextObject(
        FormatSeconds(m_ResultData.solvedTime),
        48,
        {colMid.x, -152.0f},
        solvedTimeColor,
        1.6f));
    m_Renderer.AddChild(CreateTextObject(
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

void ResultScreen::Update() {
    for (const auto& button : m_Buttons) {
        button->Update();
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_NextScreenType = ScreenType::MENU;
    }

    m_Renderer.Update();
}

ScreenType ResultScreen::GetNextScreenType() {
    return m_NextScreenType;
}

ScreenType ResultScreen::GetScreenType() const {
    return ScreenType::RESULT;
}

}  // namespace UI
