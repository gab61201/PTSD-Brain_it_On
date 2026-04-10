# CompositeObject

**檔案**: `include/GameWorld/CompositeObject.hpp`

複合物件類別，由多個 BaseObject 組合成一個整體的物理實體。

## 描述

`CompositeObject` 允許將多個基礎物件組合在一起，形成一個單一的可物理模擬的物體。所有子物件會共享同一個 Box2D Body。

## BodyType 列舉

定義了物理物件的類型：

| 值 | 說明 |
|----|------|
| `STATIC` | 靜態物件，不被物理引擎移動，也不受力影響 |
| `DYNAMIC` | 動態物件，會受到重力與力的影響而移動 |
| `KINEMATIC` | 運動學物件，不受力影響但可程式控制移動 |

## 建構式

```cpp
CompositeObject() = default;

CompositeObject(
    std::vector<std::shared_ptr<BaseObject>> baseObjects,  // 子物件列表
    BodyType bodyType = BodyType::STATIC,                  // 物體類型 (預設 STATIC)
    glm::vec2 position = {0.0F, 0.0F},                     // 位置 (預設原點)
    float rotation = 0.0F                                  // 旋轉角度 (預設 0)
);
```

## 方法

### `virtual void Update()`

更新複合物件狀態。

### `virtual void AttachToWorld(Physics::WorldPtr world)`

將此複合物件及其所有子物件掛載到 Box2D 世界中。

## 成員變數

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_Renderer` | `Util::Renderer` | 渲染器，用於繪製視覺物件 |
| `m_BaseObjects` | `std::vector<std::shared_ptr<BaseObject>>` | 子物件列表 |
| `m_BodyType` | `BodyType` | 物體類型 |
| `m_Position` | `glm::vec2` | 位置 |
| `m_Rotation` | `float` | 旋轉角度 |
| `m_Body` | `Physics::BodyPtr` | Box2D Body handle (`b2_nullBodyId` 表示未掛載) |

## 相關類別

- **BaseObject**: 基礎物件，CompositeObject 的組成單元
- **DrawnObject**: 繪製物件，繼承自 CompositeObject
- **MagnetObject**: 磁鐵物件，繼承自 CompositeObject
- **PhysicalWorld**: 物理世界管理員
