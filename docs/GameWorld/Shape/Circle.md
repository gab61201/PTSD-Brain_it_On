# Circle

**標頭檔**：[`include/GameWorld/Shape/Circle.hpp`](../../../include/GameWorld/Shape/Circle.hpp)

繼承 [`Shape`](Shape.md)。圓形碰撞形狀。

```cpp
Circle(
    float diameter,                    // 直徑（像素）
    const glm::vec2& relativePosition, // 相對位置
    bool isSensor = false
);

void AttachToBody(b2BodyId body) override;
```
