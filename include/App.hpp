#ifndef APP_HPP
#define APP_HPP

#include "Physics/CompoundPhysicalObject.hpp"
#include "Physics/PhysicalObject.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "UIManager.hpp"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "pch.hpp"  // IWYU pragma: export

class App {
   public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End();  // NOLINT(readability-convert-member-functions-to-static)

    // Compound demo
    Util::Renderer m_Root;
    PhysicsWorld m_PhysicsWorld;
    std::shared_ptr<CompoundPhysicalObject> m_Compound;
    std::shared_ptr<PhysicalObject> m_PhysFloor;

   private:
    State m_CurrentState = State::START;
    UIManager ui;
};

#endif
