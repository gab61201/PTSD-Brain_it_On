#include "Screen/MenuScreen.hpp"

#include <string>
#include <vector>

#include "Level/LevelData.hpp"
#include "Screen/UIElement.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"
#include "Util/ProgressStore.hpp"
#include "Constants.hpp"

namespace {

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



glm::vec2 GetCardPosition(int index) {
    const int column = index % MENU_COLUMNS;
    const int row = index / MENU_COLUMNS;
    return {CARD_X_START + CARD_X_STEP * static_cast<float>(column),
            CARD_Y_START + CARD_Y_STEP * static_cast<float>(row)};
}

}  // namespace

namespace UI {

MenuScreen::MenuScreen() {
    m_NextScreenType = ScreenType::MENU;

    auto background = UI::Element::Background(Path::Background);
    m_Renderer.AddChild(background);

    auto panelDrawable = std::make_shared<Util::Image>(Path::BlueSquare);
    auto panel = std::make_shared<Util::GameObject>(panelDrawable, -0.5f);
    panel->m_Transform.translation = {0.0f, 0.0f};
    panel->m_Transform.scale = {PANEL_WIDTH, PANEL_HEIGHT};
    m_Renderer.AddChild(panel);

    auto totalStarsText = UI::Element::Text(
        "Total Stars: " + std::to_string(Util::ProgressStore::GetTotalStarCount()),
        40,
        {0.0f, 230.0f},
        Util::Color::FromRGB(255, 255, 255),
        1.0f);
    m_Renderer.AddChild(totalStarsText);

    auto backButton = UI::Element::CircleButton([this]() {
        m_NextScreenType = ScreenType::LOBBY;
    },
                                                Path::BtnBack);
    backButton->m_Transform.translation = {-560.0f, -300.0f};
    m_Buttons.push_back(backButton);
    m_Renderer.AddChild(backButton);

    // 只顯示已註冊的關卡，避免點到未註冊的 LevelId 在 GetLevelConfig 中拋例外。
    // 假設關卡 ID 從 LEVEL_1(0) 連續向上註冊。
    const int menuCardCount = static_cast<int>(GetLevelRegistry().size());
    for (int index = 0; index < menuCardCount; ++index) {
        LevelId levelId = static_cast<LevelId>(index);
        std::string levelNumberStr = std::to_string(index + 1);
        const glm::vec2 cardPosition = GetCardPosition(index);
        auto cardBackground = UI::Element::Image(
            Path::LightBlueSquare,
            {cardPosition.x, cardPosition.y - 12.0f},
            {CARD_SCALE * 0.80f, CARD_SCALE * 0.80f},
            -0.1f);
        m_Renderer.AddChild(cardBackground);

        auto cardButton = UI::Element::SquareButton(
            [this, index]() {
                m_LevelId = static_cast<LevelId>(index);
                m_NextScreenType = ScreenType::GAME;
            },
            Util::ProgressStore::GetScreenshotPath(levelId));
        cardButton->m_Transform.translation = cardPosition;
        cardButton->m_Transform.scale = {THUMB_SCALE, THUMB_SCALE};
        m_Buttons.push_back(cardButton);
        m_Renderer.AddChild(cardButton);

        auto levelText = UI::Element::Text(levelNumberStr, 30,
                                          {cardPosition.x + 8.0f, cardPosition.y + 6.0f},
                                          Util::Color::FromRGB(245, 245, 245), 0.9f);
        m_Renderer.AddChild(levelText);

        const float starXOffsets[3] = {-38.0f, 0.0f, 38.0f};
        std::array<bool, 3> stars = Util::ProgressStore::GetStars(levelId);
        for (int starIndex = 0; starIndex < 3; ++starIndex) {
            std::string starPath = stars.at(starIndex) ? Path::StarBright
                                                       : Path::StarDark;
            auto starImg = UI::Element::Image(starPath,
                                             {cardPosition.x + starXOffsets[starIndex], cardPosition.y - 58.0f},
                                             {STAR_SCALE, STAR_SCALE},
                                             0.8f);
            m_Renderer.AddChild(starImg);
        }
    }
}

ScreenType MenuScreen::Update() {
    m_Renderer.Update();
    for (const auto& button : m_Buttons) {
        button->Update();
    }
    return m_NextScreenType;
}

ScreenType MenuScreen::GetScreenType() const {
    return ScreenType::MENU;
}

LevelId MenuScreen::GetSelectedLevelId() const {
    return m_LevelId;
}

}  // namespace UI