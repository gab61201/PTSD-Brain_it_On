# MagnetObject

**標頭檔**：[`include/GameWorld/CompositeObject/MagnetObject.hpp`](../../../include/GameWorld/CompositeObject/MagnetObject.hpp)

繼承 [`CompositeObject`](CompositeObject.md)。帶有磁力吸引 / 排斥效果的特殊物件。

```cpp
MagnetObject(
    std::vector<std::shared_ptr<Shape>> shapes,
    BodyType bodyType = BodyType::STATIC,
    glm::vec2 position = {0.0F, 0.0F},
    float rotation = 0.0F,
    float magnetism = 0.0F  // 正值=吸引，負值=排斥
);
```

#### 覆寫方法

| 方法 | 說明 |
|------|------|
| `AttachToWorld(b2WorldId)` | 附加到世界並儲存 world ID 以進行磁力計算 |
| `Update()` | 每幀更新磁力效果與視覺位置 |

#### 存取器

| 方法 | 回傳值 | 說明 |
|------|--------|------|
| `GetMagnetism` | `float` | 取得磁力值 |
