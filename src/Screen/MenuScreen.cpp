#include "Screen/MenuScreen.hpp"

#include <filesystem>
#include <iomanip>
#include <sstream>

#include "Level/LevelData.hpp"
#include "Progress/ProgressStore.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"

namespace {

constexpr int MENU_CARD_COUNT = 10;
constexpr int MENU_COLUMNS = 5;

constexpr float PANEL_WIDTH = 1.88f;
constexpr float PANEL_HEIGHT = 1.42f;
constexpr float CARD_SCALE = 0.40f;
constexpr float THUMB_SCALE = 0.15f;
constexpr float STAR_SCALE = 0.16f;

constexpr float CARD_X_START = -300.0f;
constexpr float CARD_X_STEP = 150.0f;
constexpr float CARD_Y_START = 100.0f;
constexpr float CARD_Y_STEP = -235.0f;

std::shared_ptr<Util::GameObject> CreateTextObject(const std::string& text,
                                                   int size,
                                                   const glm::vec2& position,
                                                   const Util::Color& color,
                                                   float z = 1.0f) {
    auto drawable = std::make_shared<Util::Text>("Resources/Fonts/Inter.ttf", size, text, color);
    auto object = std::make_shared<Util::GameObject>(drawable, z);
    object->m_Transform.translation = position;
    return object;
}

std::shared_ptr<Util::GameObject> CreateImageObject(const std::string& path,
                                                    const glm::vec2& position,
                                                    const glm::vec2& scale,
                                                    float z = 1.0f) {
    auto drawable = std::make_shared<Util::Image>(path);
    auto object = std::make_shared<Util::GameObject>(drawable, z);
    object->m_Transform.translation = position;
    object->m_Transform.scale = scale;
    return object;
}

std::string GetThumbnailPath(int levelNumber) {
    std::ostringstream thumbnailPath;
    thumbnailPath << "Resources/Save/LevelScreenshots/level_" << levelNumber << ".bmp";

    const std::string candidate = thumbnailPath.str();
    if (std::filesystem::exists(candidate)) {
        return candidate;
    }

    return "Resources/Images/level_frame.png";
}

glm::vec2 GetCardPosition(int index) {
    const int column = index % MENU_COLUMNS;
    const int row = index / MENU_COLUMNS;
    return {CARD_X_START + CARD_X_STEP * static_cast<float>(column),
            CARD_Y_START + CARD_Y_STEP * static_cast<float>(row)};
}

bool IsLevelUnlocked(int index) {
    if (index < 0) {
        return false;
    }

    const auto levelId = static_cast<LevelId>(index);
    const auto& registry = GetLevelRegistry();
    return registry.find(levelId) != registry.end();
}

}  // namespace

namespace UI {

MenuScreen::MenuScreen(LevelId* levelId, ProgressStore* progressStore)
    : m_LevelId(levelId), m_ProgressStore(progressStore) {
    m_NextScreenType = ScreenType::MENU;

    auto background = UI::Element::Background("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    auto panelDrawable = std::make_shared<Util::Image>("Resources/Images/BasicShapes/blue_square.png");
    auto panel = std::make_shared<Util::GameObject>(panelDrawable, -0.5f);
    panel->m_Transform.translation = {0.0f, 0.0f};
    panel->m_Transform.scale = {PANEL_WIDTH, PANEL_HEIGHT};
    m_Renderer.AddChild(panel);

    auto totalStarsText = CreateTextObject(
        "Total Stars: " + std::to_string(m_ProgressStore ? m_ProgressStore->GetTotalStars() : 0),
        40,
        {0.0f, 230.0f},
        Util::Color::FromRGB(255, 255, 255),
        1.0f);
    m_Renderer.AddChild(totalStarsText);

    auto backButton = UI::Element::CircleButton([this]() {
        m_NextScreenType = ScreenType::LOBBY;
    },
                                                "Resources/Images/Btn_Back.png");
    backButton->m_Transform.translation = {-560.0f, -300.0f};
    m_Buttons.push_back(backButton);
    m_Renderer.AddChild(backButton);

    for (int index = 0; index < MENU_CARD_COUNT; ++index) {
        const int levelNumber = index + 1;
        const glm::vec2 cardPosition = GetCardPosition(index);
        const bool unlocked = IsLevelUnlocked(index);
        const bool hasProgress = unlocked && m_ProgressStore != nullptr;
        const StarConditions conditions = hasProgress
                                              ? m_ProgressStore->GetConditions(static_cast<LevelId>(index))
                                              : StarConditions{false, false, false};

        auto cardBackground = CreateImageObject(
            "Resources/Images/BasicShapes/light_blue_square.png",
            {cardPosition.x, cardPosition.y - 12.0f},
            {CARD_SCALE * 0.80f, CARD_SCALE * 0.80f},
            -0.1f);
        m_Renderer.AddChild(cardBackground);

        auto cardButton = UI::Element::SquareButton(
            [this, index, unlocked]() {
                if (!unlocked || !m_LevelId) {
                    return;
                }

                *m_LevelId = static_cast<LevelId>(index);
                m_NextScreenType = ScreenType::GAME;
            },
            GetThumbnailPath(levelNumber));
        cardButton->m_Transform.translation = cardPosition;
        cardButton->m_Transform.scale = {THUMB_SCALE, THUMB_SCALE};
        m_Buttons.push_back(cardButton);
        m_Renderer.AddChild(cardButton);

        auto levelText = CreateTextObject(std::to_string(levelNumber), 30,
                                          {cardPosition.x + 8.0f, cardPosition.y + 6.0f},
                                          Util::Color::FromRGB(245, 245, 245), 0.9f);
        m_Renderer.AddChild(levelText);

        const float starXOffsets[3] = {-38.0f, 0.0f, 38.0f};
        for (int starIndex = 0; starIndex < 3; ++starIndex) {
            const std::string starPath = conditions[starIndex]
                                             ? "Resources/Images/star_bright.png"
                                             : "Resources/Images/star_dark.png";
            auto star = CreateImageObject(starPath,
                                          {cardPosition.x + starXOffsets[starIndex], cardPosition.y - 58.0f},
                                          {STAR_SCALE, STAR_SCALE},
                                          0.8f);
            m_Renderer.AddChild(star);
        }
    }
}

void MenuScreen::Update() {
    for (const auto& button : m_Buttons) {
        button->Update();
    }

    m_Renderer.Update();
}

ScreenType MenuScreen::GetNextScreenType() {
    return m_NextScreenType;
}

ScreenType MenuScreen::GetScreenType() const {
    return ScreenType::MENU;
}

}  // namespace UI