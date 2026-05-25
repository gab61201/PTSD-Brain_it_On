# ProgressStore

全靜態類別，管理玩家進度的讀取與儲存。

**標頭檔**：[`include/Util/ProgressStore.hpp`](../../include/Util/ProgressStore.hpp)

#### 靜態方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `LoadOrCreateDefault` | `static void LoadOrCreateDefault()` | 從 CSV 載入進度；若檔案不存在則建立空檔 |
| `GetStars` | `static std::array<bool,3> GetStars(LevelId)` | 取得指定關卡的三星狀態 |
| `GetTotalStarCount` | `static int GetTotalStarCount()` | 取得所有關卡的總星數 |
| `ApplyResultAndSave` | `static bool ApplyResultAndSave(const LevelResult&)` | 套用結果、更新最佳紀錄並存檔。回傳是否為新紀錄 |
| `GetScreenshotPath` | `static std::string GetScreenshotPath(LevelId)` | 取得指定關卡的截圖路徑 |

#### 靜態成員（Private）

| 變數 | 型別 | 說明 |
|------|------|------|
| `s_Records` | `std::map<LevelId, ProgressRecord>` | 所有關卡的進度記錄 |
