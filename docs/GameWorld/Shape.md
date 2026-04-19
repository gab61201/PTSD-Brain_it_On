# Shape

**檔案**: `include/GameWorld/Shape/Shape.hpp`

形狀抽象基類，定義所有物理碰撞形狀的共同介面。

## 描述

`Shape` 是遊戲中所有物理形狀的基礎類別。每個 Shape 包含一個視覺表示 (`Util::GameObject`) 和一個 Box2D 碰撞形狀 (`b2ShapeId`)。具體的子類別包括 `Circle`、`Rectangle` 與 `Capsule`。

## 建構式

```cpp
Shape() = default;

Shape(
    std::variant<glm::vec2, float> m_Size,     // 大小 (圓形為直徑 float，矩形為 vec2)
    const glm::vec2& relativePosition,          // 相對於父物件的位置
    float relativeRotation,                     // 相對於父物件的旋轉角度 (弧度)
    bool isSensor = false                       // 是否為感測器 (預設 false)
);
```

## 方法

### `virtual void AttachToBody(b2BodyId body)`

創建對應的 `b2Shape` 實例以及 GameObject 實例，並將形狀附加到 Box2D Body 上。

**純虛函式**: 子類別必須實作

**參數**:
- `body`: 要附加到的 Box2D Body handle

### `virtual void Update(glm::vec2 ParentObjectPosition, float ParentObjectRotation)`

更新 `m_Visual` 的世界座標和旋轉角度。

**參數**:
- `ParentObjectPosition`: 父物件的位置
- `ParentObjectRotation`: 父物件的旋轉角度

### `b2ShapeId Getb2ShapeId() const`

取得此 Shape 的 Box2D shape handle。

**回傳值**: `b2ShapeId`

### `std::shared_ptr<Util::GameObject> GetVisual() const`

取得視覺表示物件。

**回傳值**: `Util::GameObject` 指標

## 成員變數 (受保護)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_Visual` | `std::shared_ptr<Util::GameObject>` | 圖像表示 |
| `m_b2ShapeId` | `b2ShapeId` | Box2D shape handle (`b2_nullShapeId` 表示未建立) |
| `m_Size` | `std::variant<glm::vec2, float>` | 大小 (圓形為直徑，矩形為寬高) |
| `m_RelativePosition` | `glm::vec2` | 相對於父物件的位置 |
| `m_RelativeRotation` | `float` | 相對於父物件的旋轉角度 (弧度) |
| `m_IsSensor` | `bool` | 是否為感測器 |

## 靜態成員

### `static Util::AssetStore<std::shared_ptr<Util::Image>> s_ImageCache`

圖片快取，所有 Shape 共用。

## 繼承關係

- **子類別**:
  - `Circle`: 圓形碰撞體
  - `Rectangle`: 矩形碰撞體
  - `Capsule`: 膠囊形碰撞體

## 相關類別

- **CompositeObject**: 持有 Shape 列表的複合物件
- **CoordinateHelper**: 座標轉換工具 (像素 ↔ 公尺)
