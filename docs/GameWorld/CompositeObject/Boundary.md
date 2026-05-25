# Boundary

**標頭檔**：[`include/GameWorld/CompositeObject/Boundary.hpp`](../../../include/GameWorld/CompositeObject/Boundary.hpp)

繼承 [`CompositeObject`](CompositeObject.md)。定義關卡的矩形邊界（四面牆壁）。

```cpp
Boundary(float x1, float x2, float y1, float y2);
```

#### 方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `IsPointInside` | `bool IsPointInside(glm::vec2 position) const` | 判斷一個點是否在邊界內 |
| `GetLeftWall` | `const Shape& GetLeftWall() const` | 取得左牆 Shape 參考 |
| `GetRightWall` | `const Shape& GetRightWall() const` | 取得右牆 Shape 參考 |
| `GetTopWall` | `const Shape& GetTopWall() const` | 取得上牆 Shape 參考 |
| `GetBottomWall` | `const Shape& GetBottomWall() const` | 取得下牆 Shape 參考 |
