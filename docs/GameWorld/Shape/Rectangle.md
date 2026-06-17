# Rectangle

**標頭檔**：[`include/GameWorld/Shape/Rectangle.hpp`](../../../include/GameWorld/Shape/Rectangle.hpp)

繼承 [`Shape`](Shape.md)。矩形碰撞形狀。

```cpp
Rectangle(
    const glm::vec2& size,                    // 寬、高（像素）
    const glm::vec2& relativePosition,        // 相對位置
    float relativeRotation = 0.0f,            // 相對旋轉（弧度）
    ShapeColor color = ShapeColor::White,     // 形狀顏色
    bool isSensor = false,                    // 是否為感測器
    bool outline = true,                      // 是否顯示描邊
    bool isForbidden = false                  // 禁止繪畫區（優先於 color）
);

void AttachToBody(b2BodyId body) override;
```
