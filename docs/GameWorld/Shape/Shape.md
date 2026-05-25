# Shape

**標頭檔**：[`include/GameWorld/Shape/Shape.hpp`](../../../include/GameWorld/Shape/Shape.hpp)

所有物理形狀的抽象基底類別。每個 `Shape` 同時持有 Box2D 碰撞形狀（`b2ShapeId`）與 PTSD 視覺物件（`Util::GameObject`）。

#### 建構子

```cpp
Shape(
    std::variant<glm::vec2, float> size,  // vec2=矩形(寬,高), float=圓形(直徑)
    const glm::vec2& relativePosition,     // 相對於父物件的偏移
    float relativeRotation,                // 相對旋轉角度（弧度）
    bool isSensor = false                  // true 時只觸發事件、不產生碰撞
);
```

#### 純虛方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `AttachToBody` | `virtual void AttachToBody(b2BodyId body) = 0` | 將形狀附加到指定的 Box2D 剛體 |

#### 虛方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `Update` | `virtual void Update(glm::vec2 parentPos, float parentRot)` | 根據父物件位置 / 旋轉更新視覺物件座標 |

#### 存取器

| 方法 | 回傳值 | 說明 |
|------|--------|------|
| `Getb2ShapeId` | `b2ShapeId` | 取得 Box2D 形狀 ID |
| `GetVisual` | `std::shared_ptr<Util::GameObject>` | 取得視覺物件 |

#### Protected 成員

| 變數 | 型別 | 說明 |
|------|------|------|
| `m_Visual` | `std::shared_ptr<Util::GameObject>` | 視覺表示 |
| `m_b2ShapeId` | `b2ShapeId` | Box2D 形狀 handle |
| `m_Size` | `std::variant<glm::vec2, float>` | 大小 |
| `m_RelativePosition` | `glm::vec2` | 相對位置 |
| `m_RelativeRotation` | `float` | 相對旋轉（弧度） |
| `m_IsSensor` | `bool` | 是否為感測器 |
| `s_ImageCache` | `Util::AssetStore<...>` (static) | 圖片快取 |
