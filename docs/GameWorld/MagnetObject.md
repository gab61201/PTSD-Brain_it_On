# MagnetObject

**檔案**: `include/GameWorld/MagnetObject.hpp`

具有磁力效果的複合物件類別，繼承自 CompositeObject。

## 描述

`MagnetObject` 代表物理世界中的磁鐵物件，能夠對其他金屬物件產生吸引力或排斥力。每幀會執行磁力計算來模擬磁場效果。

## 建構式

```cpp
MagnetObject(
    std::vector<std::shared_ptr<BaseObject>> baseObjects,  // 子物件列表
    BodyType bodyType = BodyType::DYNAMIC,                 // 物體類型 (預設 DYNAMIC)
    glm::vec2 position = {0.0F, 0.0F},                     // 位置 (預設原點)
    float rotation = 0.0F,                                 // 旋轉角度 (預設 0)
    float magnetism = 0.0F                                 // 磁力值 (預設 0)
);
```

**參數說明**:
- `baseObjects`: 構成磁鐵形狀的基礎物件
- `bodyType`: 物理類型，DYNAMIC 表示磁鐵本身也會移動
- `position`: 初始位置
- `rotation`: 旋轉角度
- `magnetism`: 磁力值，正值為 N 極 (吸引 S 極)，負值為 S 極

## 方法

### `void AttachToWorld(b2World* world)`

將磁鐵掛載到物理世界。此方法會將本物件的指標存入 Box2D Body 的 UserData，以便在碰撞檢測時識別磁力來源。

**參數**:
- `world`: Box2D 世界指標

### `void Update() override`

更新磁鐵狀態，每幀執行磁力相吸邏輯。此方法重載了父類別的 Update，額外加入磁力計算。

### `[[nodiscard]] float GetMagnetism() const`

取得當前磁力值。

**回傳值**: 磁力值
- 正值：N 極
- 負值：S 極

## 成員變數

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_Magnetism` | `float` | 磁力值 (私有) |

## 繼承關係

- **繼承自**: `GameWorld::CompositeObject`
- **子類別**: 無

## 特殊行為

- 每幀更新時會計算磁場對其他物件的影響力
- UserData 機制用於快速識別磁鐵與相關物件的互動

## 相關類別

- **CompositeObject**: 父類別，提供複合物件基礎功能
- **PhysicalWorld**: 管理磁鐵物件並執行磁力模擬
