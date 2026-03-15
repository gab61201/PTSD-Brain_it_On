#include "GameWorld/PhysicalWorld.hpp"  // 請確認你的標頭檔名稱

// ==========================================
// 建構子 (Constructor)
// ==========================================
PhysicalWorld::PhysicalWorld(std::vector<std::shared_ptr<CompositeObject>> compositeObjects)
    // 1. 初始化 Box2D 世界，並設定重力。
    // 💡 提示：Box2D 預設 Y 軸向上。如果你的遊戲畫面 Y 軸是朝下的，請改成正數 (例如 0.0f, 9.8f)
    : m_b2World(b2Vec2(0.0f, -9.8f)),
      m_CompositeObject(std::move(compositeObjects)),
      m_state(state::PAUSE)  // 遊戲初始狀態設為暫停
{
    // 2. 在世界建立的瞬間，把所有初始的物件掛載到物理引擎裡
    for (auto& obj : m_CompositeObject) {
        if (obj) {
            obj->AttachToWorld(&m_b2World);
        }
    }
}

// ==========================================
// 每一幀的更新 (Update) - 遊戲主迴圈會呼叫這裡
// ==========================================
void PhysicalWorld::Update() {
    // 1. 只有在「遊玩中」的狀態，時間才會流動，物理引擎才會運作
    if (m_state == state::PLAYING) {
        // Box2D 官方建議的模擬參數
        float timeStep = 1.0f / 60.0f;  // 假設遊戲以 60 FPS 運行
        int32 velocityIterations = 8;   // 速度迭代次數 (越高越精準，但較耗效能)
        int32 positionIterations = 3;   // 位置迭代次數 (解決物體重疊)

        // 🚀 讓物理世界往前推演一步！
        m_b2World.Step(timeStep, velocityIterations, positionIterations);
    }

    // 2. 同步畫面座標
    // 💡 提示：就算在 PAUSE 或 PLAYER_DRAWING 狀態，我們還是要呼叫 Update()，
    // 這樣畫面上的物件才不會消失，只是它們會停在原地不動。
    for (auto& obj : m_CompositeObject) {
        if (obj) {
            obj->Update();
        }
    }
}

// ==========================================
// 開始遊戲 (Start)
// ==========================================
void PhysicalWorld::Start() {
    m_state = state::PLAYING;
}

// ==========================================
// 暫停遊戲 (Stop)
// ==========================================
void PhysicalWorld::Stop() {
    m_state = state::PAUSE;
}

// ==========================================
// 玩家繪圖模式 (PlayerDrawObject)
// ==========================================
void PhysicalWorld::PlayerDrawObject() {
    // 切換狀態，此時 Update() 裡的 m_b2World.Step() 會停止，畫面凍結
    m_state = state::PLAYER_DRAWING;

    // TODO: 這裡未來要實作「玩家滑鼠畫線」的邏輯。
    // 大致流程會是：
    // 1. 記錄滑鼠軌跡點。
    // 2. 將軌跡轉換成多個 BaseObject (例如用一堆小矩形連成一條線)。
    // 3. 把這些 BaseObject 包裝成一個新的 CompositeObject。
    // 4. newCompositeObj->AttachToWorld(&m_b2World);
    // 5. m_CompositeObject.push_back(newCompositeObj);
}

std::vector<std::shared_ptr<Util::GameObject>> PhysicalWorld::GetVisuals() const {
    std::vector<std::shared_ptr<Util::GameObject>> visuals;

    for (const auto& obj : m_CompositeObject) {
        if (!obj) {
            continue;
        }

        auto objectVisuals = obj->GetVisuals();
        visuals.insert(visuals.end(), objectVisuals.begin(), objectVisuals.end());
    }

    return visuals;
}