# LevelHUD

**標頭檔**：[`include/Level/LevelHUD.hpp`](../../include/Level/LevelHUD.hpp)

關卡內的抬頭顯示器（HUD），負責計時器、筆畫限制、提示文字等 UI 元素的顯示。

```cpp
LevelHUD(LevelId levelId, const std::string& targetText, int strokeLimit);
```

#### 方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `UpdateTimer` | `void UpdateTimer(float remainingTime)` | 更新計時器顯示 |
| `HideTarget` | `void HideTarget()` | 隱藏提示文字（玩家開始繪圖時） |
| `Reset` | `void Reset(const string& targetText, int strokeLimit)` | 重置 HUD |
| `UpdateStrokeLimit` | `void UpdateStrokeLimit(int remaining, int total)` | 更新筆畫限制顯示 |
| `UpdateContactTimer` | `void UpdateContactTimer(int contactCountDown)` | 更新接觸倒數計時 |
| `Update` | `void Update()` | 繪製所有 HUD 元素 |
