#include "Screen/MenuScreen.hpp"

namespace UI {

MenuScreen::MenuScreen(LevelId* levelId) : m_LevelId(levelId) {
    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "MenuScreen"));
    m_Renderer.AddChild(title);
}

// 關卡入口加在這
ScreenType MenuScreen::Update() {
    m_Renderer.Update();

    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        *m_LevelId = LevelId::LEVEL_1;
        return ScreenType::GAME;
    } else if (Util::Input::IsKeyUp(Util::Keycode::NUM_2)) {
        *m_LevelId = LevelId::LEVEL_2;
        return ScreenType::GAME;
    } else if (Util::Input::IsKeyUp(Util::Keycode::NUM_3)) {
        *m_LevelId = LevelId::LEVEL_3;
        return ScreenType::GAME;
    } else if (Util::Input::IsKeyUp(Util::Keycode::NUM_4)) {
        *m_LevelId = LevelId::LEVEL_4;
        return ScreenType::GAME;
    } else if (Util::Input::IsKeyUp(Util::Keycode::NUM_5)) {
        *m_LevelId = LevelId::LEVEL_5;
        return ScreenType::GAME;
    }

    return ScreenType::MENU;
}

}  // namespace UI