# DrawingIndicator

**標頭檔**：[`include/GameWorld/DrawingIndicator.hpp`](../../include/GameWorld/DrawingIndicator.hpp)

玩家繪製時的即時視覺回饋指示器，在兩點之間顯示線段。

```cpp
DrawingIndicator();
```

#### 方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `DrawLine` | `void DrawLine(const glm::vec2& p1, const glm::vec2& p2)` | 在兩點之間繪製指示線段 |
| `Update` | `void Update()` | 更新渲染 |
