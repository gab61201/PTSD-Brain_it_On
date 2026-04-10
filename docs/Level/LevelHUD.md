# LevelHUD

**檔案**: `include/Level/LevelHUD.hpp`

關卡介面上的 HUD (Heads-Up Display) 類別，負責顯示遊戲資訊。

## 描述

`LevelHUD` 管理畫面上方的使用者介面元素，包括時間計時器、目標提示文字與筆劃限制顯示。這些資訊協助玩家了解當前關卡的進度與目標。

## 建構式

```cpp
LevelHUD(LevelId levelId, const std::string& targetText, int strokeLimit);
```

**參數**:
- `levelId`: 關卡 ID，用於識別當前關卡
- `targetText`: 顯示給玩家的目標提示文字 (如：「將球放入盒子」)
- `strokeLimit`: 當前關卡的筆劃限制，用於初始化 HUD 顯示

## 方法

### `void UpdateTimer(float remainingTime)`

每幀更新計時器顯示。根據剩餘時間動態調整顯示內容與顏色。

**參數**:
- `remainingTime`: 剩餘秒數

### `void HideTarget()`

隱藏目標提示文字。在玩家開始繪圖時呼叫，避免干擾視覺。

### `void Reset(const std::string& targetText, int strokeLimit)`

重置 HUD 元素，用於重新開始關卡時，同時更新筆劃限制顯示。

**參數**:
- `targetText`: 新的目標提示文字
- `strokeLimit`: 新的筆劃限制

### `void UpdateStrokeLimit(int remainingStroke, int totalStrokeLimit)`

每幀更新筆劃限制顯示。文字格式固定為 `remaining/total`，例如 `3/3`、`0/3`、`-1/3`。

**參數**:
- `remainingStroke`: 剩餘筆劃數，可為負數
- `totalStrokeLimit`: 當前關卡的總筆劃限制

### `void Update()`

繪製所有 HUD 元素到螢幕上。此方法會呼叫渲染器將文字與物件顯示出來。

## 成員變數

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_Renderer` | `Util::Renderer` | 渲染器，用於繪製 HUD 元素 |
| `m_TimerText` | `std::shared_ptr<Util::Text>` | 計時器文字物件 |
| `m_TimerObject` | `std::shared_ptr<Util::GameObject>` | 計時器的視覺容器 |
| `m_TargetTextObject` | `std::shared_ptr<Util::GameObject>` | 目標提示文字的視覺容器 |
| `m_StrokeLimitText` | `std::shared_ptr<Util::Text>` | 筆劃限制文字物件 |
| `m_StrokeLimitObject` | `std::shared_ptr<Util::GameObject>` | 筆劃限制文字的視覺容器 |

## HUD 顯示元素

### 計時器
- **位置**: 螢幕上方中央
- **內容**: 剩餘時間 (秒)
- **更新頻率**: 每幀

### 目標文字
- **位置**: 螢幕下方或適當位置
- **內容**: 關卡目標提示
- **行為**: 開始繪圖時自動隱藏

### 筆劃限制
- **位置**: 限制筆劃圖示下方
- **內容**: `remaining/total` 格式的剩餘筆劃數
- **行為**: 可顯示負數，不阻擋玩家繼續繪圖

## 相關類別

- **Level**: Level 會持有 HUD 並呼叫其 UpdateTimer 與 UpdateStrokeLimit 方法
- **Util::Renderer**: 用於渲染 HUD 元素
- **Util::Text**: 文字顯示物件
- **Util::GameObject**: 遊戲物件基類，HUD 元素的容器
