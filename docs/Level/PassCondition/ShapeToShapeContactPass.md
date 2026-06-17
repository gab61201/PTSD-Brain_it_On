# ShapeToShapeContactPass

**標頭檔**：[`include/Level/PassCondition/ShapeToShapeContactPass.hpp`](../../../include/Level/PassCondition/ShapeToShapeContactPass.hpp)

`ShapeToShapeContactPass` 繼承自 `PassCondition`，用於設定「指定的兩個形狀之間發生接觸事件」作為過關條件。

## 建構子

```cpp
ShapeToShapeContactPass(
    b2ShapeId shapeA,
    b2ShapeId shapeB,
    TriggerType triggerType,
    int duration);
```

- **`shapeA`**: 第一個目標形狀 ID。
- **`shapeB`**: 第二個目標形狀 ID。
- **`triggerType`**: 觸發接觸的條件類型，例如 `TOUCHING`。
- **`duration`**: 必須維持該觸發狀態的幀數（或時間），達到後即視為過關。

建構時會自動為 `shapeA` 與 `shapeB` 啟用接觸或感測事件（`b2Shape_EnableContactEvents` 或 `b2Shape_EnableSensorEvents`）。
