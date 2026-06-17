# GroupToShapeContactPass

**標頭檔**：[`include/Level/PassCondition/GroupToShapeContactPass.hpp`](../../../include/Level/PassCondition/GroupToShapeContactPass.hpp)

`GroupToShapeContactPass` 繼承自 `PassCondition`，用於設定「一組形狀中的任何一個，與另一個指定形狀發生接觸事件」作為過關條件（多對一的接觸條件）。

## 建構子

```cpp
GroupToShapeContactPass(
    std::vector<b2ShapeId> shapeGroup,
    b2ShapeId targetShape,
    TriggerType triggerType,
    int duration);
```

- **`shapeGroup`**: 備選的形狀陣列。只要其中任意一個與 `targetShape` 接觸即可觸發。
- **`targetShape`**: 單一目標形狀 ID。
- **`triggerType`**: 觸發接觸的條件類型，例如 `TOUCHING`。
- **`duration`**: 必須維持該觸發狀態的幀數（或時間），達到後即視為過關。

建構時會自動為 `shapeGroup` 中的所有形狀以及 `targetShape` 啟用接觸或感測事件（`b2Shape_EnableContactEvents` 或 `b2Shape_EnableSensorEvents`）。
