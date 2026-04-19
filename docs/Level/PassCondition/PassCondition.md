# PassCondition

**檔案**: `include/Level/PassCondition/PassCondition.hpp`

過關條件基類，定義了所有過關檢測機制的共同介面。

## 描述

`PassCondition` 是用來判斷玩家是否達成關卡目標的抽象類別。它每幀從 Box2D v3 的 contact events 緩衝讀取資料並根據預設條件判定過關與否。

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

### `bool Check(b2ContactEvents events)`

檢查當前是否已達成過關條件。每幀從 Box2D 取得 contact events 並傳入此方法進行檢測。內部會呼叫 `OnContactEvent` 轉發事件給子類別處理，並根據持續時間判定是否過關。

**參數**:
- `events`: Box2D v3 的 contact events (`b2ContactEvents`)

**回傳值**: 
- `true`: 過關條件已達成
- `false`: 尚未達成

## 受保護成員

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_TriggerType` | `TriggerType` | 觸發類型 |
| `m_Duration` | `int` | 需要持續的幀數 |
| `m_Timer` | `int` | 當前計時器值 |

## 純虛函式

### `virtual void OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) = 0`

處理碰撞事件的子類別實作。當指定的碰撞事件發生時呼叫。

**參數**:
- `shapeA`: 第一個碰撞 shape handle (`b2ShapeId`)
- `shapeB`: 第二個碰撞 shape handle (`b2ShapeId`)
- `triggerType`: 觸發類型 (TOUCHING / SEPARATED)

## 繼承關係

- **基類**: 無 (獨立抽象類別)
- **子類別**:
  - `OneToOneContactPass`: 雙物件接觸檢測

## 使用流程

```cpp
// 1. 建立過關條件
auto condition = std::make_shared<OneToOneContactPass>(shapeA, shapeB, TriggerType::TOUCHING, duration);

// 2. 每幀從 PhysicalWorld 取得碰撞事件並檢查
b2ContactEvents events = physicalWorld->GetContactEvents();
if (condition->Check(events)) {
    // 達成過關條件
}
```

## 相關類別

- **OneToOneContactPass**: 檢測兩個特定物件的接觸狀態
- **TriggerType**: 觸發類型列舉
- **b2ContactEvents**: Box2D v3 的每幀碰撞事件資料
- **PhysicalWorld**: 提供 contact events
