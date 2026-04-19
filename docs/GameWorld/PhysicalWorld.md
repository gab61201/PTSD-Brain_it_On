# PhysicalWorld

**檔案**: `include/GameWorld/PhysicalWorld.hpp`

物理世界管理員，負責協調所有物理物件與碰撞檢測。

## 描述

`PhysicalWorld` 是遊戲物理系統的控制中心，整合了 Box2D 物理引擎、玩家繪製的物件、邊界檢測等功能。

## 建構式

```cpp
PhysicalWorld(
    std::vector<std::shared_ptr<CompositeObject>> compositeObjects,  // 地圖原有物件
    std::shared_ptr<Boundary> boundary                               // 邊界物件
);
```

**參數**:
- `compositeObjects`: 關卡設計時預先放置的複合物件列表
- `boundary`: 地圖邊界物件，用於檢測物件是否在範圍內

## 方法

### `void Start()`

啟動物理世界，開始模擬與渲染。建立 Box2D world 並掛載所有物件。

### `void Stop()`

停止物理世界運作。銷毀 Box2D world。

### `void DrawObject(glm::vec2 position)`

在指定位置開始繪製新物件。此方法會建立一個新的 DrawnObject 並加入管理列表。

**參數**:
- `position`: 繪製起始位置

### `void EndDrawing()`

完成當前繪製作業，生成最終的物理碰撞體。

### `void Update()`

每幀更新物理世界狀態，包括：
- 執行 Box2D 物理模擬 (`b2World_Step`)
- 更新所有物件位置
- 更新繪製指示器 (DrawingIndicator)

### `int GetDrawnObjectCount() const`

取得玩家繪製的物件數量。

**回傳值**: DrawnObject 數量

### `b2ContactEvents GetContactEvents()`

取得當前幀的碰撞事件資料，供過關條件檢測使用。

**回傳值**: Box2D v3 的 contact events

## 成員變數 (私有)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_b2WorldId` | `b2WorldId` | Box2D v3 世界 handle |
| `m_CompositeObject` | `std::vector<std::shared_ptr<CompositeObject>>` | 地圖預先放置的物件 |
| `m_Boundary` | `std::shared_ptr<Boundary>` | 邊界物件 |
| `m_DrawnObjects` | `std::vector<std::shared_ptr<DrawnObject>>` | 玩家繪製的物件列表 |
| `m_LastDrawingObject` | `std::shared_ptr<DrawnObject>` | 當前正在繪製的物件 |
| `m_DrawingIndicator` | `DrawingIndicator` | 繪製指示器 (視覺提示) |
| `m_IsActive` | `bool` | 物理世界是否處於活動狀態 |

## 更新流程

```
每幀執行順序:
1. Start/Stop - 啟動或停止模擬
2. DrawObject - 玩家開始繪製
3. Update - Box2D 物理模擬 + DrawingIndicator 更新
4. GetContactEvents - 取得碰撞事件供 PassCondition 使用
5. EndDrawing - 完成繪製作業
```

## 相關類別

- **CompositeObject**: 地圖中預先放置的物理物件類型
- **DrawnObject**: 玩家繪製的動態物件
- **Boundary**: 地圖邊界，提供 `IsPointInside()` 檢測
- **DrawingIndicator**: 繪製時的視覺指示器
- **PassCondition**: 過關條件 (由 Level 管理)
