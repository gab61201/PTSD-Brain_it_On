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
constexpr float STAR_DARK_SCALE = 0.12f;

constexpr float CARD_X_START = -300.0f;
constexpr float CARD_X_STEP = 150.0f;
constexpr float CARD_Y_START = 130.0f;
constexpr float CARD_Y_STEP = -160.0f;



glm::vec2 GetCardPosition(int index) {
    const int column = index % MENU_COLUMNS;
    const int row = index / MENU_COLUMNS;
    return {CARD_X_START + CARD_X_STEP * static_cast<float>(column),
            CARD_Y_START + CARD_Y_STEP * static_cast<float>(row)};
}

}  // namespace

namespace UI {

MenuScreen::MenuScreen() : UIScreen(ScreenType::MENU) {

    auto background = UI::Element::Background(Path::Background);
    m_Renderer.AddChild(background);

    auto panelDrawable = std::make_shared<Util::Image>(Path::BlueSquare);
    auto panel = std::make_shared<Util::GameObject>(panelDrawable, Layer::UIBackground);
    panel->m_Transform.translation = {0.0f, 0.0f};
    panel->m_Transform.scale = {PANEL_WIDTH, PANEL_HEIGHT};
    m_Renderer.AddChild(panel);

    auto totalStarsText = UI::Element::Text(
        "Total Stars: " + std::to_string(Util::ProgressStore::GetTotalStarCount()),
        40,
        {0.0f, 230.0f},
        Util::Color::FromRGB(255, 255, 255),
        Layer::UIElement);
    m_Renderer.AddChild(totalStarsText);

    auto backButton = UI::Element::Button(Path::BtnBack, [this]() {
        m_NextScreenType = ScreenType::LOBBY;
    });
    backButton->m_Transform.translation = {-560.0f, -300.0f};
    m_Buttons.push_back(backButton);
    m_Renderer.AddChild(backButton);

    int uiIndex = 0;
    for (auto const& [levelId, func] : GetLevelRegistry()) {
        int levelNumber = static_cast<int>(levelId) + 1;
        std::string levelNumberStr = std::to_string(levelNumber);
        const glm::vec2 cardPosition = GetCardPosition(uiIndex);
        auto cardBackground = UI::Element::Image(
            Path::LightBlueSquare,
            {cardPosition.x, cardPosition.y - 12.0f},
            {CARD_SCALE * 0.80f, CARD_SCALE * 0.80f},
            Layer::UIOutline);
        m_Renderer.AddChild(cardBackground);

        auto cardButton = UI::Element::Button(
            Util::ProgressStore::GetScreenshotPath(levelId),
            [this, levelId]() {
                m_LevelId = levelId;
                m_NextScreenType = ScreenType::GAME;
            });
        cardButton->m_Transform.translation = cardPosition;
        cardButton->m_Transform.scale = {THUMB_SCALE, THUMB_SCALE};
        m_Buttons.push_back(cardButton);
        m_Renderer.AddChild(cardButton);

        auto levelText = UI::Element::Text(levelNumberStr, 30,
                                          {cardPosition.x + 8.0f, cardPosition.y + 6.0f},
                                          Util::Color::FromRGB(245, 245, 245), Layer::UIElementHUD);
        m_Renderer.AddChild(levelText);

        const float starXOffsets[3] = {-38.0f, 0.0f, 38.0f};
        std::array<bool, 3> stars = Util::ProgressStore::GetStars(levelId);
        for (int starIndex = 0; starIndex < 3; ++starIndex) {
            std::string starPath = stars.at(starIndex) ? Path::StarBright
                                                       : Path::StarDark;
            float scale = stars.at(starIndex) ? STAR_SCALE : STAR_DARK_SCALE;
            auto starImg = UI::Element::Image(starPath,
                                             {cardPosition.x + starXOffsets[starIndex], cardPosition.y - 58.0f},
                                             {scale, scale},
                                             Layer::UIElementHUD);
            m_Renderer.AddChild(starImg);
        }
        uiIndex++;
    }
}

ScreenType MenuScreen::Update() {
    m_Renderer.Update();
    for (const auto& button : m_Buttons) {
        button->Update();
    }
    return m_NextScreenType;
}

}  // namespace UI