# CompositeObject

**檔案**: `include/GameWorld/CompositeObject/CompositeObject.hpp`

複合物件類別，由多個 Shape 組合成一個整體的物理實體。

## 描述

`CompositeObject` 允許將多個 Shape 物件組合在一起，形成一個單一的可物理模擬的物體。所有子 Shape 會共享同一個 Box2D Body。

## BodyType 列舉

定義了物理物件的類型：

| 值 | 說明 |
|----|------|
| `STATIC` | 靜態物件，不被物理引擎移動，也不受力影響 (`b2_staticBody`) |
| `KINEMATIC` | 運動學物件，不受力影響但可程式控制移動 (`b2_kinematicBody`) |
| `DYNAMIC` | 動態物件，會受到重力與力的影響而移動 (`b2_dynamicBody`) |

## 建構式

```cpp
CompositeObject() = default;

CompositeObject(
    std::vector<std::shared_ptr<Shape>> shapes,   // Shape 列表
    BodyType bodyType = BodyType::STATIC,          // 物體類型 (預設 STATIC)
    glm::vec2 position = {0.0F, 0.0F},             // 位置 (預設原點)
    float rotation = 0.0F                          // 旋轉角度 (預設 0)
);
```

## 方法

### `virtual void Update()`

更新複合物件狀態。每幀遞迴呼叫所有子 Shape 的 `Update()` 以同步視覺位置與旋轉。

**覆寫自**: 無 (虛擬函式)

### `virtual void AttachToWorld(b2WorldId world)`

將此複合物件及其所有子物件掛載到 Box2D 世界中。建立 `b2Body` 並將每個 Shape 附加其上。

**參數**:
- `world`: Box2D 世界 handle

## 成員變數 (受保護)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_Renderer` | `Util::Renderer` | 渲染器，用於繪製視覺物件 |
| `m_Shapes` | `std::vector<std::shared_ptr<Shape>>` | Shape 子物件列表 |
| `m_BodyType` | `BodyType` | 物體類型 |
| `m_Position` | `glm::vec2` | 位置 |
| `m_Rotation` | `float` | 旋轉角度 |
| `m_b2BodyId` | `b2BodyId` | Box2D Body handle (`b2_nullBodyId` 表示未掛載) |

## 繼承關係

- **子類別**:
  - `DrawnObject`: 玩家繪製的物件
  - `Boundary`: 地圖邊界物件

## 相關類別

- **Shape**: 基礎形狀抽象，CompositeObject 的組成單元
- **Circle**: 圓形 Shape
- **Rectangle**: 矩形 Shape
- **Capsule**: 膠囊形 Shape
- **PhysicalWorld**: 物理世界管理員
