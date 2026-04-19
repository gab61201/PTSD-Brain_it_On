# Rectangle

**檔案**: `include/GameWorld/Shape/Rectangle.hpp`

矩形碰撞體，繼承自 Shape。

## 描述

`Rectangle` 代表一個矩形的物理碰撞體。大小參數為寬高 (vec2)。

## 建構式

```cpp
Rectangle(const glm::vec2& size, const glm::vec2& relativePosition, float relativeRotation = 0.0f, bool isSensor = false);
```

**參數**:
- `size`: 矩形寬高 (像素)
- `relativePosition`: 相對於父物件的位置
- `relativeRotation`: 相對於父物件的旋轉角度 (預設 0)
- `isSensor`: 是否為感測器 (預設 false)

## 方法

### `void AttachToBody(b2BodyId body) override`

創建矩形 `b2Shape` 並附加到 Box2D Body 上。使用 `b2CreatePolygonShape()` 建立碰撞體，尺寸轉換為公尺。

**參數**:
- `body`: 要附加到的 Box2D Body handle

## 繼承關係

- **繼承自**: `GameWorld::Shape`

## 相關類別

- **Shape**: 形狀抽象基類
- **Circle**: 圓形碰撞體
- **Capsule**: 膠囊形碰撞體
