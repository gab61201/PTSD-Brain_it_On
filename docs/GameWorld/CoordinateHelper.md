# CoordinateHelper

**檔案**: `include/GameWorld/CoordinateHelper.hpp`

座標轉換工具函式，用於在像素 (Pixels) 與公尺 (Meters) 之間進行轉換。

## 描述

物理引擎 Box2D 使用公尺作為單位，而遊戲渲染使用像素。此模組提供轉換函式來處理兩者之間的對應關係。

## 常數

| 名稱 | 值 | 說明 |
|------|-----|------|
| `kPixelsPerMeter` | `50.0F` | 每公尺相當於的像素數量 |

## 轉換函式

### `b2Vec2 PixelsToMeters(glm::vec2 pixels)`

將像素座標轉換為公尺座標。

**參數**:
- `pixels`: 像素座標

**回傳值**: Box2D 向量形式的公尺座標

### `float PixelsToMeters(float pixels)`

將像素距離轉換為公尺距離。

**參數**:
- `pixels`: 像素距離

**回傳值**: 公尺單位距離

### `glm::vec2 MetersToPixels(b2Vec2 meters)`

將公尺座標轉換為像素座標。

**參數**:
- `meters`: Box2D 向量形式的公尺座標

**回傳值**: GLM 向量形式的像素座標

## 使用範例

```cpp
// 將螢幕上的點擊位置轉換為物理世界座標
glm::vec2 screenPos = GetMousePosition();
b2Vec2 physicsPos = GameWorld::PixelsToMeters(screenPos);

// 將物理引擎回傳的座標轉換為渲染座標
b2Vec2 bodyPos = m_Body->GetPosition();
glm::vec2 renderPos = GameWorld::MetersToPixels(bodyPos);
```

## 相關類別

- **BaseObject**: 使用此轉換函式處理位置
- **PhysicalWorld**: 在建立物理世界時進行座標轉換
