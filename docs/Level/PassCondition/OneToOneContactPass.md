# OneToOneContactPass

**檔案**: `include/Level/PassCondition/OneToOneContactPass.hpp`

特定的過關條件：當兩個指定物件接觸 (或分離) 時達成。

## 描述

`OneToOneContactPass` 是 `PassCondition` 的具體實作，用於檢測兩個特定基礎物件之間的互動狀態。當它們達到指定的觸發條件並持續足夠時間後，即視為過關。

## 建構式

### 版本一：雙物件接觸

```cpp
OneToOneContactPass(
    std::shared_ptr<GameWorld::BaseObject> baseObjectA,   // 物件 A
    std::shared_ptr<GameWorld::BaseObject> baseObjectB,   // 物件 B
    TriggerType triggerType,                              // 觸發類型：接觸或分離
    int duration                                          // 需要持續的幀數
);
```

### 版本二：單一物件自我檢測

```cpp
OneToOneContactPass(
    std::shared_ptr<GameWorld::BaseObject> baseObject,    // 物件
    TriggerType triggerType,                              // 觸發類型
    int duration                                          // 需要持續的幀數
);
```

**參數說明**:
- `baseObjectA/B`: 要檢測的兩個基礎物件指標
- `triggerType`: 接觸 (TOUCHING) 或分離 (SEPARATED)
- `duration`: 條件需維持的幀數，防止誤判

## 方法

### `void AttachToWorld(b2WorldId world)`

將過關條件掛載到物理世界，並同步儲存目標物件的 shape handle。

**參數**:
- `world`: Box2D 世界 handle

### `void OnContactEvent(b2ShapeId fixtureA, b2ShapeId fixtureB, TriggerType triggerType) override`

處理碰撞事件的內部實作。當指定的兩個 shape 發生接觸或分離時呼叫。

**參數**:
- `fixtureA`: 第一個碰撞 shape handle
- `fixtureB`: 第二個碰撞 shape handle
- `triggerType`: 觸發類型

## 成員變數

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_BaseObjectA` | `std::shared_ptr<GameWorld::BaseObject>` | 物件 A (私有) |
| `m_BaseObjectB` | `std::shared_ptr<GameWorld::BaseObject>` | 物件 B (私有，可為 nullptr) |
| `m_FixtureA` | `b2ShapeId` | Shape A handle (內部使用) |
| `m_FixtureB` | `b2ShapeId` | Shape B handle (內部使用，可為 null) |

## 繼承關係

- **繼承自**: `PassCondition`
- **父類別方法**:
  - `AttachToWorld()`: 掛載到物理世界
  - `ConsumeContactEvents()`: 讀取本幀碰撞事件
  - `Update()`: 更新計時器
  - `Check()`: 檢查是否過關

## 使用範例

```cpp
// 創建一個 OneToOneContactPass，當球體放入盒子內即過關
auto ball = std::make_shared<BaseObject>(...);
auto box = std::make_shared<BaseObject>(...);

auto passCondition = new OneToOneContactPass(
    ball,      // 球體物件
    box,       // 盒子物件
    TriggerType::TOUCHING,  // 接觸時觸發
    60         // 需要持續 60 幀 (約 1 秒)
);
```

## 相關類別

- **PassCondition**: 過關條件基類，定義通用介面
- **TriggerType**: 觸發類型列舉 (TOUCHING, SEPARATED)
- **BaseObject**: 基礎物件，代表物理世界中的實體
- **PhysicalWorld**: 管理過關條件並執行檢測
