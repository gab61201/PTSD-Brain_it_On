#include "App.hpp"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    m_PhysicsWorld.InitializeScene();

    m_Circle = std::make_shared<Util::GameObject>();
    m_Circle->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/white_circle.png"));
    m_Circle->m_Transform.scale = {0.1f, 0.1f};
    m_Circle->SetZIndex(50);
    m_Root.AddChild(m_Circle);

    m_Floor = std::make_shared<Util::GameObject>();
    m_Floor->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/white_floor.png"));
    m_Floor->m_Transform.translation = {0, -225};
    m_Floor->SetZIndex(0);
    m_Root.AddChild(m_Floor);

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    ui.Update();

    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() {  // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
