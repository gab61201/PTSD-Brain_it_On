# DrawingIndicator

**檔案**: `include/GameWorld/DrawingIndicator.hpp`

繪製指示器，提供玩家在繪製階段時的視覺回饋。

## 描述

`DrawingIndicator` 在玩家繪製物件時顯示兩個端點與連接線段，讓玩家清楚知道當前繪製的路徑。

## 建構式

```cpp
DrawingIndicator();
```

## 方法

### `void DrawLine(const glm::vec2& p1, const glm::vec2& p2)`

在兩點之間繪製一條線段。更新指示器的 P1、P2 位置與連線視覺。

**參數**:
- `p1`: 第一點的像素座標
- `p2`: 第二點的像素座標

### `void Update()`

更新繪製指示器。每幀呼叫以同步視覺物件的位置。

## 成員變數 (私有)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `s_ImageCache` | `static Util::AssetStore<std::shared_ptr<Util::Image>>` | 圖片快取 |
| `m_Renderer` | `Util::Renderer` | 渲染器 |
| `m_P1` | `std::shared_ptr<Util::GameObject>` | 第一點視覺物件 |
| `m_P2` | `std::shared_ptr<Util::GameObject>` | 第二點視覺物件 |
| `m_Line` | `std::shared_ptr<Util::GameObject>` | 連接線段視覺物件 |

## 相關類別

- **DrawnObject**: 使用 DrawingIndicator 提供繪製時的視覺回饋
- **PhysicalWorld**: 持有 DrawingIndicator 並每幀更新
