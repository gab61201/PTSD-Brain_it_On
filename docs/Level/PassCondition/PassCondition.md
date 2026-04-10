# PassCondition

**檔案**: `include/Level/PassCondition/PassCondition.hpp`

過關條件基類，定義了所有過關檢測機制的共同介面。

## 描述

`PassCondition` 是用來判斷玩家是否達成關卡目標的抽象類別。它不再繼承 Box2D listener，而是每幀從 Box2D v3 的 contact events 緩衝讀取資料並根據預設條件判定過關與否。

## TriggerType 列舉

定義了觸發過關條件的兩種狀態：

| 值 | 說明 |
|----|------|
| `TOUCHING` | 物件需要相互接觸才能達成條件 |
| `SEPARATED` | 物件需要相互分離才能達成條件 |

## 建構式

```cpp
PassCondition() = default;

PassCondition(TriggerType triggerType, int duration);
```

**參數**:
- `triggerType`: 觸發類型 (接觸或分離)
- `duration`: 條件需維持的幀數，防止誤判

## 方法

### `virtual void AttachToWorld(b2WorldId world)`

將過關條件掛載到指定的物理世界。此為純虛函式，子類別必須實作。

**參數**:
- `world`: Box2D 世界 handle

**回傳值**: 無 (虛函式)

### `void ConsumeContactEvents(b2WorldId world)`

從 Box2D v3 世界讀取本幀 contact begin/end 事件，並轉發給 `OnContactEvent`。

**參數**:
- `world`: Box2D 世界 handle

### `void Update()`

每幀更新過關條件狀態，包括計時器遞增等邏輯。

### `bool Check() const`

檢查當前是否已達成過關條件。

**回傳值**: 
- `true`: 過關條件已達成
- `false`: 尚未達成

## 受保護成員

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_TriggerType` | `TriggerType` | 觸發類型 |
| `m_Duration` | `int` | 需要持續的幀數 |
| `m_IsPassed` | `bool` | 是否已過關 (內部狀態) |
| `m_Timer` | `int` | 當前計時器值 |

## 純虛函式

### `virtual void OnContactEvent(b2ShapeId fixtureA, b2ShapeId fixtureB, TriggerType triggerType)`

處理碰撞事件的子類別實作。當指定的碰撞事件發生時呼叫。

**參數**:
- `fixtureA`: 第一個碰撞 shape handle
- `fixtureB`: 第二個碰撞 shape handle
- `triggerType`: 觸發類型

## 繼承關係

- **基類**: 無 (獨立抽象類別)
- **子類別**:
  - `OneToOneContactPass`: 雙物件接觸檢測
  - 其他自定義過關條件類別

## 使用流程

```cpp
// 1. 建立過關條件
auto condition = new MyCustomCondition(...);

// 2. 掛載到物理世界
condition->AttachToWorld(worldId);

// 3. 每幀先消費接觸事件，再更新
condition->ConsumeContactEvents(worldId);
condition->Update();

// 4. 檢查是否過關
if (condition->Check()) {
    // 達成過關條件
}
```

## 相關類別

- **OneToOneContactPass**: 檢測兩個特定物件的接觸狀態
- **TriggerType**: 觸發類型列舉
- **b2ContactEvents**: Box2D v3 的每幀碰撞事件資料
- **PhysicalWorld**: 管理過關條件的生命週期
