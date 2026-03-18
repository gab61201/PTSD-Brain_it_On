#include "GameWorld/PhysicalWorld.hpp"

#include "GameWorld/BaseObject.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

#define STROKE_WIDTH 10.0F

namespace GameWorld {

PhysicalWorld::PhysicalWorld(std::vector<std::shared_ptr<CompositeObject>> compositeObjects)
    : m_state(state::PAUSE),
      m_b2World(b2Vec2(0.0f, -9.8f)),
      m_CompositeObject(std::move(compositeObjects)) {
    for (auto& obj : m_CompositeObject) {
        if (obj) {
            obj->AttachToWorld(&m_b2World);
        }
    }
}

void PhysicalWorld::Playing() {
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        m_state = state::PLAYER_DRAWING;
        auto newDrawnObject = std::make_shared<DrawnObject>(Util::Input::GetCursorPosition());
        newDrawnObject->AttachToWorld(&m_b2World);
        m_DrawnObjects.push_back(newDrawnObject);
    } else if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        m_state = state::PAUSE;
    }
}

void PhysicalWorld::PlayerDrawObject() {
    if (m_DrawnObjects.empty()) {
        m_state = state::PLAYING;
        return;
    }

    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        m_DrawnObjects.back()->EndDrawing();
        m_state = state::PLAYING;
        return;
    }
    auto nowDrawingObject = m_DrawnObjects.back();
    nowDrawingObject->DrawNextPoint(Util::Input::GetCursorPosition());

    // TODO: 這裡未來要實作「玩家滑鼠畫線」的邏輯。
    // 大致流程會是：
    // 1. 記錄滑鼠軌跡點。
    // 2. 將軌跡轉換成多個 BaseObject (例如用一堆小矩形連成一條線)。
    // 3. 把這些 BaseObject 包裝成一個新的 CompositeObject。
    // 4. newCompositeObj->AttachToWorld(&m_b2World);
    // 5. m_CompositeObject.push_back(newCompositeObj);
}

void PhysicalWorld::Pause() {
    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        m_state = state::PLAYING;
    }
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB) && m_DrawnObjects.empty()) {
        m_state = state::PLAYER_DRAWING;
        auto newDrawnObject = std::make_shared<DrawnObject>(Util::Input::GetCursorPosition());
        newDrawnObject->AttachToWorld(&m_b2World);
        m_DrawnObjects.push_back(newDrawnObject);
    }
}

// ==========================================
// 每一幀的更新 (Update) - 遊戲主迴圈會呼叫這裡
// ==========================================
void PhysicalWorld::Update() {
    float timeStep = 1.0f / 60.0f;  // 假設遊戲以 60 FPS 運行
    int32 velocityIterations = 8;   // 速度迭代次數 (越高越精準，但較耗效能)
    int32 positionIterations = 3;   // 位置迭代次數 (解決物體重疊)
    switch (m_state) {
        case state::PLAYING:
            Playing();
            m_b2World.Step(timeStep, velocityIterations, positionIterations);
            break;
        case state::PLAYER_DRAWING:
            PlayerDrawObject();
            m_b2World.Step(timeStep, velocityIterations, positionIterations);
            break;
        case state::PAUSE:
            Pause();
            break;
    }
    // 2. 同步畫面座標
    for (auto& obj : m_CompositeObject) {
        if (obj) {
            obj->Update();
        }
    }
    for (auto& obj : m_DrawnObjects) {
        if (obj) {
            obj->Update();
        }
    }
    
}

}  // namespace GameWorld
