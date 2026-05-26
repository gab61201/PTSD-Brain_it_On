# CoordinateHelper

**標頭檔**：[`include/GameWorld/CoordinateHelper.hpp`](../../include/GameWorld/CoordinateHelper.hpp)

提供像素座標與 Box2D 公尺座標之間的轉換工具。

#### 常數

| 常數 | 值 | 說明 |
|------|-----|------|
| `kPixelsPerMeter` | `50.0` | 每公尺對應的像素數 |

#### 函式

```cpp
// 將像素座標向量轉換為 Box2D 公尺座標
b2Vec2 PixelsToMeters(glm::vec2 pixels);

// 將單一像素值轉換為公尺
float PixelsToMeters(float pixels);

// 將 Box2D 公尺座標轉換為像素座標
glm::vec2 MetersToPixels(b2Vec2 meters);
```
