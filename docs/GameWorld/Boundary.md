# Boundary

**檔案**: `include/GameWorld/CompositeObject/Boundary.hpp`

地圖邊界物件，繼承自 CompositeObject。

## 描述

`Boundary` 代表遊戲地圖的邊界範圍，由四個座標 (x1, x2, y1, y2) 定義一個矩形區域。用於檢測玩家繪製的物件是否在有效範圍內。

## 建構式

```cpp
Boundary(float x1, float x2, float y1, float y2);
```

**參數**:
- `x1`: 左邊界 X 座標
- `x2`: 右邊界 X 座標
- `y1`: 下邊界 Y 座標
- `y2`: 上邊界 Y 座標

## 方法

### `bool IsPointInside(glm::vec2 position) const`

檢查指定座標是否在邊界範圍內。

**參數**:
- `position`: 要檢測的像素座標

**回傳值**:
- `true`: 座標在邊界範圍內
- `false`: 座標在邊界範圍外

## 成員變數 (私有)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_X1` | `float` | 左邊界 X 座標 |
| `m_X2` | `float` | 右邊界 X 座標 |
| `m_Y1` | `float` | 下邊界 Y 座標 |
| `m_Y2` | `float` | 上邊界 Y 座標 |

## 繼承關係

- **繼承自**: `GameWorld::CompositeObject`

## 相關類別

- **CompositeObject**: 父類別
- **PhysicalWorld**: 持有 Boundary 物件並用於範圍檢測
