#include "App.hpp"

#include "Physics/CompoundPhysicalObject.hpp"
#include "Physics/PhysicalObject.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    // === 靜態地板 ===
    m_PhysFloor = PhysicalObject::CreateRectangle(
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
    // 直條寬 30，高 150 (半寬高為 15, 75)
    auto bar1 = std::make_shared<Util::GameObject>();
    bar1->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/Images/square.png"));
    bar1->m_Transform.scale = {30.0F / 417.0F, 150.0F / 417.0F};
    bar1->SetZIndex(50);
    m_Compound->AddBoxChild(
        m_PhysicsWorld, bar1,
        {0.0F, 40.0F},        // 本地偏移：往上
        {15.0F, 75.0F}        // 半寬半高
    );

    // L 型的橫條部分
    // 橫條寬 120，高 30 (半寬高為 60, 15)
    // 橫條下沿對齊直條下沿： Y = 40 (直條中心) - 75 (直條半高) + 15 (橫條半高) = -20
    // 首端從直條左側延伸： X = 0 (直條中心) - 15 (直條半寬) + 60 (橫條半寬) = 45
    auto bar2 = std::make_shared<Util::GameObject>();
    bar2->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/Images/square.png"));
    bar2->m_Transform.scale = {120.0F / 417.0F, 30.0F / 417.0F};
    bar2->SetZIndex(50);
    m_Compound->AddBoxChild(
        m_PhysicsWorld, bar2,
        {45.0F, -20.0F},      // 本地偏移
        {60.0F, 15.0F}        // 半寬半高
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
