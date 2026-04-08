# BaseObject

**檔案**: `include/GameWorld/BaseObject.hpp`

物理物件的基礎類別，定義了所有 Box2D 實體物件的共同介面。

## 描述

`BaseObject` 負責將視覺物件與物理世界中的固定 (Fixture) 連結起來。每個物件都包含一個視覺表示 (`GameObject`) 和一個物理碰撞體 (`b2Fixture`)。

## 建構式

```cpp
BaseObject(
    ShapeType shape,           // 形狀類型：圓形、矩形或三角形
    glm::vec2 size,            // 物件大小
    glm::vec2 position,        // 位置
    float rotation = 0.0F,     // 旋轉角度 (可選，預設為 0)
    bool isSensor = false      // 是否為感應器 (可選，預設為 false)
);
```

## 方法

### `void Update(glm::vec2 ParentObjectPosition, float ParentObjectRotation)`

更新物件的相對位置與旋轉。用於當此物件是其他物件的子物件時。

**參數**:
- `ParentObjectPosition`: 父物件的位置
- `ParentObjectRotation`: 父物件的旋轉角度

### `void AttachToBody(b2Body* body)`

將此物件掛載到 Box2D Body 上，建立視覺與物理世界的連結。

**參數**:
- `body`: 要掛載到的 Box2D Body 指標

## 成員變數

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_Visual` | `std::shared_ptr<Util::GameObject>` | 視覺物件 |
| `m_Fixture` | `b2Fixture*` | Box2D 固定器 (碰撞體) |

## 受保護成員

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_ShapeType` | `ShapeType` | 形狀類型 |
| `m_Size` | `glm::vec2` | 物件大小 |
| `m_RelativePosition` | `glm::vec2` | 相對位置 |
| `m_RelativeRotation` | `float` | 相對旋轉 |
| `m_IsSensor` | `bool` | 是否為感應器 |

## 相關類別

- **ShapeType**: 形狀類型列舉 (CIRCLE, RECTANGLE, TRIANGLE)
- **CompositeObject**: 複合物件，由多個 BaseObject 組成
- **PhysicalWorld**: 物理世界管理員
