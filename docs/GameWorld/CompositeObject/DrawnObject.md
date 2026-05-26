# DrawnObject

**標頭檔**：[`include/GameWorld/CompositeObject/DrawnObject.hpp`](../../../include/GameWorld/CompositeObject/DrawnObject.hpp)

繼承 [`CompositeObject`](CompositeObject.md)。玩家繪製產生的動態物件，由一系列座標點組成。

```cpp
DrawnObject(glm::vec2 position);  // 起始點
```

#### 方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `DrawNextPoint` | `void DrawNextPoint(glm::vec2 position)` | 新增一個繪製點 |
| `EndDrawing` | `void EndDrawing()` | 結束繪製，將點轉換為 Box2D 剛體 |
| `GetPoints` | `const vector<glm::vec2>& GetPoints() const` | 取得所有繪製點 |
