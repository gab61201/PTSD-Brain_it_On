#include "App.hpp"

#include "Physics/CompoundPhysicalObject.hpp"
#include "Physics/PhysicalObject.hpp"
#include "Physics/PhysicalRectangle.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    // === 靜態地板 ===
    m_PhysFloor = PhysicalRectangle::Create(
        m_PhysicsWorld,
        {0.0F, -225.0F},  // 位置
        {300.0F, 50.0F},  // 寬高
        0.0F,             // 旋轉
        false             // 靜態
    );
    m_Root.AddChild(m_PhysFloor->GetVisual());

    // === 動態 L 型 Compound 物件 ===
    m_Compound = CompoundPhysicalObject::Create(
        m_PhysicsWorld,
        {0.0F, 200.0F},  // 初始位置
        -0.1F,            // 初始旋轉（稍微傾斜，讓掉落更有趣）
        true             // 動態
    );

    // L 型的直條部分
    m_Compound->AddRectangle(
        m_PhysicsWorld,
        {0.0F, 40.0F},        // 本地偏移：往上
        {30.0F, 150.0F},      // 寬高
        0.0f,                 // 旋轉
        RESOURCE_DIR "/Images/square.png"
    );

    // L 型的橫條部分
    m_Compound->AddRectangle(
        m_PhysicsWorld,
        {45.0F, -20.0F},      // 本地偏移
        {120.0F, 30.0F},      // 寬高
        0.0f,                 // 旋轉
        RESOURCE_DIR "/Images/square.png"
    );

    // 把 compound 的 root visual（含子物件）加入 renderer
    m_Root.AddChild(m_Compound->GetVisual());

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    // 物理模擬
    m_PhysicsWorld.Step();

    // 同步 Box2D → PTSD 視覺
    m_PhysFloor->Sync();
    m_Compound->Sync();

    // 渲染
    m_Root.Update();

    // UI
    ui.Update();

    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() {  // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
