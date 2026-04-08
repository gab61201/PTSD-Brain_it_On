# PhysicalWorld

**檔案**: `include/GameWorld/PhysicalWorld.hpp`

物理世界管理員，負責協調所有物理物件、碰撞檢測與過關條件。

## 描述

`PhysicalWorld` 是遊戲物理系統的控制中心，整合了 Box2D 物理引擎、玩家繪製的物件、磁鐵互動以及過關條件檢查。

## 建構式

```cpp
PhysicalWorld(
    std::vector<std::shared_ptr<CompositeObject>> compositeObjects,  // 地圖原有物件
    PassCondition* passCondition                                     // 過關條件指標
);
```

**參數**:
- `compositeObjects`: 關卡設計時預先放置的複合物件列表
- `passCondition`: 指向當前關卡的過關條件物件

## 方法

### `void Start()`

啟動物理世界，開始模擬與渲染。

### `void Stop()`

停止物理世界運作。

### `void DrawObject(glm::vec2 position)`

在指定位置開始繪製新物件。此方法會建立一個新的 DrawnObject 並加入管理列表。

**參數**:
- `position`: 繪製起始位置

### `void EndDrawing()`

完成當前繪製作業，生成最終的物理碰撞體。

### `bool IsPassed()`

檢查關卡是否已通過。

**回傳值**: 
- `true`: 過關條件達成
- `false`: 尚未過關

### `void Update()`

每幀更新物理世界狀態，包括：
- 執行 Box2D 物理模擬
- 更新所有物件位置
- 檢查碰撞事件
- 更新過關條件計時器

## 成員變數

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_b2World` | `b2World` | Box2D 物理世界實體 |
| `m_CompositeObject` | `std::vector<std::shared_ptr<CompositeObject>>` | 地圖預先放置的物件 |
| `m_DrawnObjects` | `std::vector<std::shared_ptr<DrawnObject>>` | 玩家繪製的物件列表 |
| `m_LastDrawingObject` | `std::shared_ptr<DrawnObject>` | 當前正在繪製的物件 |
| `m_PassCondition` | `PassCondition*` | 過關條件指標 |
| `m_IsActive` | `bool` | 物理世界是否處於活動狀態 |

## 更新流程

```
每幀執行順序:
1. Start/Stop - 啟動或停止模擬
2. DrawObject - 玩家開始繪製
3. Update - 物理模擬與碰撞檢測
4. IsPassed - 檢查是否達成過關條件
5. EndDrawing - 完成繪製作業
```

## 相關類別

- **CompositeObject**: 地圖中預先放置的物理物件類型
- **DrawnObject**: 玩家繪製的動態物件
- **MagnetObject**: 具有磁力效果的物件
- **PassCondition**: 過關條件基類
- **OneToOneContactPass**: 兩個特定物件接觸即過關的條件
