#include "Screen/LobbyScreen.hpp"

#include "GameWorld/CompositeObject/Boundary.hpp"
#include "Screen/UIElement.hpp"
#include "Constants.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

namespace UI {

void LobbyScreen::ResetPlayground() {
    auto boundary = std::make_shared<GameWorld::Boundary>(-300.0F, 300.0F, -300.0F, 200.0F);
    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary};
    m_World = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    m_World->Start();
}

LobbyScreen::LobbyScreen() : UIScreen(ScreenType::LOBBY) {

    auto background = UI::Element::Background(Path::Background);
    m_Renderer.AddChild(background);

    auto title = UI::Element::Text(
        "Brain It On!", 72, {0.0f, 280.0f}, Util::Color::FromRGB(255, 255, 255), Layer::UIElement, "Resources/Fonts/KaushanScript-Regular.ttf");
    m_Renderer.AddChild(title);

    auto titleShadow = UI::Element::Text(
        "Brain It On!", 72, {3.0f, 277.0f}, Util::Color::FromRGB(64, 64, 64), Layer::UIOutline, "Resources/Fonts/KaushanScript-Regular.ttf");
    m_Renderer.AddChild(titleShadow);

    // Retry 按鈕（清空畫布）
    auto retryButton = UI::Element::Button(Path::BtnRetry, [this]() {
        ResetPlayground();
    });
    retryButton->m_Transform.translation = {-560.0f, -300.0f};
    m_Buttons.push_back(retryButton);
    m_Renderer.AddChild(retryButton);

    // 進入 MenuScreen 的按鈕
    auto menuButton = UI::Element::Button(Path::BtnNext, [this]() {
        m_NextScreenType = ScreenType::MENU;
    });
    menuButton->m_Transform.translation = {560.0f, -300.0f};
    menuButton->m_Transform.scale = {0.6f, 0.6f};
    m_Buttons.push_back(menuButton);
    m_Renderer.AddChild(menuButton);

    ResetPlayground();
}

ScreenType LobbyScreen::Update() {
    m_Renderer.Update();

    // 先更新按鈕
    for (auto button : m_Buttons) {
        button->Update();
    }

    // 檢查游標是否在任何按鈕上，避免繪圖與按鈕衝突
    bool cursorOnButton = false;
    for (const auto& button : m_Buttons) {
        glm::vec2 pos = button->m_Transform.translation;
        glm::vec2 size = button->GetScaledSize();
        glm::vec2 cursor = Util::Input::GetCursorPosition();
        if (cursor.x >= pos.x - size.x / 2.0f && cursor.x <= pos.x + size.x / 2.0f &&
            cursor.y >= pos.y - size.y / 2.0f && cursor.y <= pos.y + size.y / 2.0f) {
            cursorOnButton = true;
            break;
        }
    }

    // 繪圖邏輯（游標不在按鈕上時才觸發）
    if (!cursorOnButton) {
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            m_World->DrawNewObject(Util::Input::GetCursorPosition());
        } else if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
            m_World->DrawingObject(Util::Input::GetCursorPosition());
        } else if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
            m_World->EndDrawing();
        }
    }

    m_World->Update();
    return m_NextScreenType;
}

}  // namespace UI