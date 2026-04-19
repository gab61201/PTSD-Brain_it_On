# Circle

**檔案**: `include/GameWorld/Shape/Circle.hpp`

圓形碰撞體，繼承自 Shape。

## 描述

`Circle` 代表一個圓形的物理碰撞體。大小參數為直徑 (diameter)。

## 建構式

```cpp
Circle(float diameter, const glm::vec2& relativePosition, bool isSensor = false);
```

**參數**:
- `diameter`: 圓形直徑 (像素)
- `relativePosition`: 相對於父物件的位置
- `isSensor`: 是否為感測器 (預設 false)

## 方法

### `void AttachToBody(b2BodyId body) override`

創建圓形 `b2Shape` 並附加到 Box2D Body 上。使用 `b2CreateCircleShape()` 建立碰撞體，半徑為直徑的一半轉換為公尺。

**參數**:
- `body`: 要附加到的 Box2D Body handle

## 繼承關係

- **繼承自**: `GameWorld::Shape`

## 相關類別

- **Shape**: 形狀抽象基類
- **Rectangle**: 矩形碰撞體
- **Capsule**: 膠囊形碰撞體
