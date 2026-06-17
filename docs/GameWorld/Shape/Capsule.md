# Capsule

**標頭檔**：[`include/GameWorld/Shape/Capsule.hpp`](../../../include/GameWorld/Shape/Capsule.hpp)

繼承 [`Shape`](Shape.md)。膠囊形碰撞形狀，由兩端圓心與直徑定義。

```cpp
Capsule(
    float diameter,                    // 直徑（像素）
    const glm::vec2& pointA,           // 左端圓心相對位置
    const glm::vec2& pointB,           // 右端圓心相對位置
    ShapeColor color = ShapeColor::White,   // 形狀顏色
    bool isSensor = false,             // 是否為感測器
    bool outline = true,               // 是否顯示描邊
    bool isForbidden = false           // 禁止繪畫區（優先於 color）
);

void AttachToBody(b2BodyId body) override;
void Update(glm::vec2 parentPos, float parentRot) override;
```

#### Private 成員

| 變數 | 型別 | 說明 |
|------|------|------|
| `m_PointA` | `glm::vec2` | 端點 A |
| `m_PointB` | `glm::vec2` | 端點 B |
| `m_CircleAVisual` | `std::shared_ptr<Util::GameObject>` | 端點 A 的圓形視覺 |
| `m_CircleBVisual` | `std::shared_ptr<Util::GameObject>` | 端點 B 的圓形視覺 |
