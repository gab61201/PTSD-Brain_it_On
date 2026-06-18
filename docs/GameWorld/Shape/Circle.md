# Circle

**標頭檔**：[`include/GameWorld/Shape/Circle.hpp`](../../../include/GameWorld/Shape/Circle.hpp)

繼承 [`Shape`](Shape.md)。圓形碰撞形狀。

```cpp
Circle(
    float diameter,                    // 直徑（像素）
    const glm::vec2& relativePosition, // 相對位置
    ShapeColor color = ShapeColor::White,   // 形狀顏色
    bool isSensor = false,             // 是否為感測器
    bool outline = true,               // 是否顯示描邊
    bool isForbidden = false           // 禁止繪畫區（優先於 color）
);

void AttachToBody(b2BodyId body) override;
```
