#ifndef UI_SCREEN_HPP
#define UI_SCREEN_HPP

#include "Util/GameObject.hpp"

namespace UI {

enum class ScreenType {
    LOBBY,
    MENU,
    GAME,
    SETTINGS
};

class UIScreen {
   public:
    virtual ~UIScreen() = default;

    virtual void Enter() {}

    virtual ScreenType Update() = 0;

    virtual void Render() {
        for (const auto &object : m_GameObjects) {
            if (object != nullptr) {
                object->Draw();
            }
        }
    }

    virtual void Exit() { m_GameObjects.clear(); }

   protected:
    void AddGameObject(const std::shared_ptr<Util::GameObject> &object) {
        m_GameObjects.push_back(object);
    }

    std::vector<std::shared_ptr<Util::GameObject>> m_GameObjects;
};

} // namespace UI

#endif