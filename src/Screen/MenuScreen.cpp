#include "Screen/MenuScreen.hpp"

#include "Level/LevelData.hpp"
#include "Progress/ProgressStore.hpp"
#include "Util/Color.hpp"

namespace UI {

MenuScreen::MenuScreen(LevelId* levelId, ProgressStore* progressStore)
    : m_LevelId(levelId), m_ProgressStore(progressStore) {
    m_NextScreenType = ScreenType::MENU;

    auto background = UI::Element::Background("Resources/Images/background.png");
    m_Renderer.AddChild(background);

    // Top: Total Stars Display
    auto totalStars = m_ProgressStore ? m_ProgressStore->GetTotalStars() : 0;
    auto totalStarsText = std::make_shared<Util::Text>
        ("PTSD/assets/fonts/Inter.ttf", 48, 
         "Total Stars: " + std::to_string(totalStars),
         Util::Color::FromRGB(255, 255, 255));
    auto totalStarsObj = std::make_shared<Util::GameObject>(totalStarsText, 1);
    totalStarsObj->m_Transform.translation = {0.0f, 300.0f};
    m_Renderer.AddChild(totalStarsObj);

    // Back button (bottom-left)
    auto backButton = UI::Element::CircleButton([this]() {
        m_NextScreenType = ScreenType::LOBBY;
    }, "Resources/Images/Btn_Back.png");
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
    const float ySpacing = 80.0f;

    for (int levelNum : levelNumbers) {
        const LevelId levelId = static_cast<LevelId>(levelNum);
        const int bestStars = m_ProgressStore->GetBestStars(levelId);
        const std::string levelLabel = "Level " + std::to_string(levelNum + 1);
        const std::string starsLabel = "★ " + std::to_string(bestStars) + "/3";

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
        auto levelText = std::make_shared<Util::Text>
            ("PTSD/assets/fonts/Inter.ttf", 32, levelLabel,
             Util::Color::FromRGB(255, 255, 255));
        auto levelTextObj = std::make_shared<Util::GameObject>(levelText, 2);
        levelTextObj->m_Transform.translation = {-100.0f, yPosition};
        m_Renderer.AddChild(levelTextObj);

        // Stars label
        auto starsText = std::make_shared<Util::Text>
            ("PTSD/assets/fonts/Inter.ttf", 32, starsLabel,
             Util::Color::FromRGB(255, 215, 0));
        auto starsTextObj = std::make_shared<Util::GameObject>(starsText, 2);
        starsTextObj->m_Transform.translation = {100.0f, yPosition};
        m_Renderer.AddChild(starsTextObj);

        yPosition -= ySpacing;
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