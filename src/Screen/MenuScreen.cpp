#include "Screen/MenuScreen.hpp"

#include "Level/LevelData.hpp"
#include "Progress/ProgressStore.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"

namespace UI {

MenuScreen::MenuScreen(LevelId* levelId, ProgressStore* progressStore)
    : m_LevelId(levelId), m_ProgressStore(progressStore) {
    m_NextScreenType = ScreenType::MENU;

    auto background = UI::Element::Background("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    auto panelDrawable = std::make_shared<Util::Image>("Resources/Images/BasicShapes/blue_square.png");
    auto panel = std::make_shared<Util::GameObject>(panelDrawable, -0.5f);
    panel->m_Transform.translation = {0.0f, 0.0f};
    panel->m_Transform.scale = {1.72f, 1.18f};
    m_Renderer.AddChild(panel);

    // Top: Total Stars Display
    auto totalStars = m_ProgressStore ? m_ProgressStore->GetTotalStars() : 0;
    auto totalStarsText = std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48,
                                                       "Total Stars: " + std::to_string(totalStars),
                                                       Util::Color::FromRGB(255, 255, 255));
    auto totalStarsObj = std::make_shared<Util::GameObject>(totalStarsText, 1);
    totalStarsObj->m_Transform.translation = {0.0f, 300.0f};
    m_Renderer.AddChild(totalStarsObj);

    // Back button (bottom-left)
    auto backButton = UI::Element::CircleButton([this]() {
        m_NextScreenType = ScreenType::LOBBY;
    },
                                                "Resources/Images/Btn_Back.png");
    backButton->m_Transform.translation = {-560.0f, -300.0f};
    m_Buttons.push_back(backButton);
    m_Renderer.AddChild(backButton);

    // Dynamic level list from registry
    if (!m_ProgressStore) {
        return;
    }

    auto& registry = GetLevelRegistry();
    std::vector<int> levelNumbers;
    for (const auto& [levelId, func] : registry) {
        levelNumbers.push_back(static_cast<int>(levelId));
    }
    std::sort(levelNumbers.begin(), levelNumbers.end());

    // Layout levels vertically
    float yPosition = 200.0f;

    for (int levelNum : levelNumbers) {
        const LevelId levelId = static_cast<LevelId>(levelNum);
        const StarConditions conditions = m_ProgressStore->GetConditions(levelId);
        const std::string levelLabel = "Level " + std::to_string(levelNum + 1);

        // Level button
        auto levelButton = UI::Element::SquareButton(
            [this, levelId]() {
                *m_LevelId = levelId;
                m_NextScreenType = ScreenType::GAME;
            },
            "Resources/Images/Btn_MainButton_Gray.png");
        levelButton->m_Transform.translation = {0.0f, yPosition};
        m_Buttons.push_back(levelButton);
        m_Renderer.AddChild(levelButton);

        // Level label
        auto levelText = std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 32, levelLabel,
                                                      Util::Color::FromRGB(255, 255, 255));
        auto levelTextObj = std::make_shared<Util::GameObject>(levelText, 2);
        levelTextObj->m_Transform.translation = {-100.0f, yPosition};
        m_Renderer.AddChild(levelTextObj);

        // Stars display
        const float starXOffsets[3] = {-20.0f, 30.0f, 80.0f};
        for (int i = 0; i < 3; ++i) {
            const std::string starPath = conditions[i]
                                             ? "Resources/Images/star_bright.png"
                                             : "Resources/Images/star_dark.png";
            auto starDrawable = std::make_shared<Util::Image>(starPath);
            auto starObj = std::make_shared<Util::GameObject>(starDrawable, 2);
            starObj->m_Transform.translation = {100.0f + starXOffsets[i], yPosition};
            starObj->m_Transform.scale = {0.20f, 0.20f};
            m_Renderer.AddChild(starObj);
        }
    }
}

void MenuScreen::Update() {
    for (auto button : m_Buttons) {
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