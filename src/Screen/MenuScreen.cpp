#include "Screen/MenuScreen.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Text.hpp"

namespace {
std::shared_ptr<Util::GameObject> CreateTitle(const std::string& text) {
    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, text));
    return title;
}
}  // namespace

namespace UI {

void MenuScreen::Initialize() {
    AddGameObject(CreateTitle("Menu"));
}

// 關卡入口加在這
ScreenType MenuScreen::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        m_LevelId = LevelId::LEVEL_1;
        return ScreenType::GAME;
    }

    return ScreenType::MENU;
}

}  // namespace UI