# ShapeToAnythingContactPass

**標頭檔**：[`include/Level/PassCondition/ShapeToAnythingContactPass.hpp`](../../../include/Level/PassCondition/ShapeToAnythingContactPass.hpp)

`ShapeToAnythingContactPass` 繼承自 `PassCondition`，用於設定「單一指定形狀與世界上任何其他形狀（包含玩家畫的物件）發生接觸事件」作為過關條件。

## 建構子

```cpp
ShapeToAnythingContactPass(
    b2ShapeId shape,
    TriggerType triggerType,
    int duration);
```

- **`shape`**: 指定的目標形狀 ID。只要它與任意其他物體接觸即觸發。
- **`triggerType`**: 觸發接觸的條件類型，例如 `TOUCHING`。
- **`duration`**: 必須維持該觸發狀態的幀數（或時間），達到後即視為過關。

建構時會自動為傳入的 `shape` 啟用接觸或感測事件（`b2Shape_EnableContactEvents` 或 `b2Shape_EnableSensorEvents`）。
