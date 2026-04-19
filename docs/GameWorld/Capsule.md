# Capsule

**檔案**: `include/GameWorld/Shape/Capsule.hpp`

膠囊形碰撞體，繼承自 Shape。

## 描述

`Capsule` 代表一個膠囊形狀 (兩端為圓弧、中間為矩形的組合) 的物理碰撞體。由兩個端點 (PointA, PointB) 和直徑定義。

## 建構式

```cpp
Capsule(
    float diameter,
    const glm::vec2& pointA,
    const glm::vec2& pointB,
    bool isSensor = false
);
```

**參數**:
- `diameter`: 膠囊寬度 (兩端圓弧的直徑)
- `pointA`: 第一端圓心座標
- `pointB`: 第二端圓心座標
- `isSensor`: 是否為感測器 (預設 false)

## 方法

### `void AttachToBody(b2BodyId body) override`

創建膠囊形 `b2Shape` 並附加到 Box2D Body 上。使用 Box2D v3 的 capsule API 建立碰撞體。

**參數**:
- `body`: 要附加到的 Box2D Body handle

### `void Update(glm::vec2 ParentObjectPosition, float ParentObjectRotation) override`

更新膠囊形視覺物件的世界座標與旋轉角度。由於膠囊形有兩個端點，需要分別更新兩個圓形的視覺位置。

**參數**:
- `ParentObjectPosition`: 父物件的位置
- `ParentObjectRotation`: 父物件的旋轉角度

## 成員變數 (私有)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_PointA` | `glm::vec2` | 第一端圓心座標 |
| `m_PointB` | `glm::vec2` | 第二端圓心座標 |
| `m_CircleAVisual` | `std::shared_ptr<Util::GameObject>` | A 端圓形視覺物件 |
| `m_CircleBVisual` | `std::shared_ptr<Util::GameObject>` | B 端圓形視覺物件 |

## 繼承關係

- **繼承自**: `GameWorld::Shape`

## 相關類別

- **Shape**: 形狀抽象基類
- **Circle**: 圓形碰撞體
- **Rectangle**: 矩形碰撞體
