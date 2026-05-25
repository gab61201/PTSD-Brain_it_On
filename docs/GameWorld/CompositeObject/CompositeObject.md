# CompositeObject

**標頭檔**：[`include/GameWorld/CompositeObject/CompositeObject.hpp`](../../../include/GameWorld/CompositeObject/CompositeObject.hpp)

由多個 `Shape` 組合而成的物理剛體。

#### `BodyType` 列舉

```cpp
enum class BodyType {
    STATIC    = b2_staticBody,    // 靜態：不受力影響
    KINEMATIC = b2_kinematicBody, // 運動學：可設定速度但不受力
    DYNAMIC   = b2_dynamicBody    // 動態：完全物理模擬
};
```

#### 建構子

```cpp
CompositeObject(
    std::vector<std::shared_ptr<Shape>> shapes,    // 子形狀列表
    BodyType bodyType = BodyType::STATIC,          // 剛體類型
    glm::vec2 position = {0.0F, 0.0F},             // 世界座標位置
    float rotation = 0.0F                          // 初始旋轉角度
);
```

#### 虛方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `Update` | `virtual void Update()` | 更新所有子形狀的視覺位置 |
| `AttachToWorld` | `virtual void AttachToWorld(b2WorldId world)` | 在 Box2D 世界建立剛體並附加所有形狀 |

#### 存取器

| 方法 | 回傳值 | 說明 |
|------|--------|------|
| `Getb2BodyId` | `b2BodyId` | 取得 Box2D 剛體 ID |
| `GetShapes` | `const vector<shared_ptr<Shape>>&` | 取得子形狀列表 |
