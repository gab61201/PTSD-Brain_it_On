# ProgressStore

## 概述
ProgressStore 提供基於 JSON 的持久化進度追蹤。它管理各關最佳星星數量，強制執行「只升不降」策略，並在檔案損毀時提供回退機制。

## 檔案位置
- **存檔路徑**：`Resources/Save/progress.json`
- **預設建立**：於 `App::Start()` 時透過 `LoadOrCreateDefault()`

## JSON 架構
```json
{
  "levels": {
    "1": 2,
    "2": 3,
    "3": 0
  }
}
```
- **鍵**：關卡 ID 字串（1 起始：「1」= LEVEL_0，「2」= LEVEL_1 等）
- **值**：達成的最佳星星 (0-3)

## 公開 API

### 建構子
```cpp
explicit ProgressStore(const std::filesystem::path& savePath = DefaultSavePath());
```
- **預設路徑**：經由 RESOURCE_DIR 巨集解析為 `Resources/Save/progress.json`

### 核心方法

#### `LoadOrCreateDefault()`
- **用途**：從磁碟初始化或建立預設空結構
- **行為**：
  - 若檔案不存在：建立 `Resources/Save/` 目錄並填入 `{"levels": {}}`
  - 若檔案存在且有效：解析 JSON 並載入最佳星星
  - 若檔案損毀/無效：記錄警告，回退至空進度，覆寫成有效 JSON
- **安全性**：損毀檔案不會造成崩潰；始終保持有效 JSON 狀態

#### `Save() const`
- **用途**：將目前進度持久化至磁碟
- **格式**：寫入美化 JSON（2 格空白縮排）
- **回傳**：成功為 `true`，檔案寫入失敗為 `false`
- **錯誤處理**：若目錄無法建立或檔案不可寫，記錄警告

#### `GetBestStars(LevelId levelId) const`
- **用途**：查詢特定關卡的最佳星星
- **回傳**：0-3，或 0 若該關卡不在地圖中（視為未遊玩）

#### `UpdateBestStars(LevelId levelId, int stars)`
- **用途**：若新星星數超過舊紀錄則更新
- **邏輯**：
  - 將輸入鎖定在 [0, 3] 範圍
  - 僅於 `stars > GetBestStars(levelId)` 時更新
  - 成功時記錄升級
- **回傳**：更新則 `true`，降級嘗試或無變化則 `false`

#### `GetTotalStars() const`
- **用途**：累加所有關卡的最佳星星
- **回傳**：聚合整數（0 ~= 無進度，3N ~= 全滿分）

#### `static CalculateStars(const LevelResultData& resultData)`
- **用途**：從單次遊戲結果計算得分星星
- **計算**：
  ```cpp
  (resultData.passed ? 1 : 0) +
  (IsWithinTimeLimit(resultData) ? 1 : 0) +
  (IsWithinStrokeLimit(resultData) ? 1 : 0)
  ```
- **星星規則**：三個獨立條件；{1,3} 可在不含 {2} 時達成
- **回傳**：0-3

## 整合點

### App 生命週期
```cpp
// src/App.cpp - App::Start()
m_ProgressStore.LoadOrCreateDefault();

// src/App.cpp - App::Update() RESULT 轉場
case UI::ScreenType::RESULT:
    LevelResultData resultData{};
    if (currentScreenType == UI::ScreenType::GAME &&
        static_cast<UI::GameScreen*>(m_Screen.get())->TryGetResultData(&resultData)) {
        int stars = ProgressStore::CalculateStars(resultData);
        if (m_ProgressStore.UpdateBestStars(resultData.levelId, stars)) {
            LOG_INFO("最佳星星更新：關卡={} 星星={}", ...);
        }
        m_ProgressStore.Save();  // 立即持久化
    }
```

### MenuScreen 整合
```cpp
// src/Screen/MenuScreen.cpp
MenuScreen::MenuScreen(LevelId* levelId, ProgressStore* progressStore)
    : m_ProgressStore(progressStore) {
    auto totalStars = m_ProgressStore ? m_ProgressStore->GetTotalStars() : 0;
    // 顯示總星星數和各關最佳星星
}
```

## 錯誤處理

### 損毀回退機制
- **觸發**：JSON 解析失敗（格式無效、缺少「levels」鍵等）
- **回應**：
  1. 記錄包含例外訊息的警告
  2. 清空記憶體中的地圖
  3. 將有效預設 JSON 寫入磁碟
  4. 以零進度繼續執行
- **結果**：損毀存檔不會導致遊戲崩潰

### 檔案 I/O 失敗
- **目錄建立失敗**：已記錄，但遊戲繼續（存檔可能僅失敗）
- **檔案寫入失敗**：回傳 `false`，呼叫端處理（通常記錄警告）
- **檔案讀取失敗**：視為損毀回退

## 實作細節

### 私有輔助函式

#### `static std::filesystem::path DefaultSavePath()`
- 解析 `RESOURCE_DIR` + `/Save/progress.json`
- 需要 CMake 定義 `RESOURCE_DIR`（Debug 編譯時設定）

#### `static std::string LevelKey(LevelId levelId)`
- 將 `LevelId` 列舉轉換為 JSON 字串鍵
- 公式：`std::to_string(static_cast<int>(levelId) + 1)`
- 範例：`LEVEL_0` → `"1"`，`LEVEL_1` → `"2"`

#### `static int ClampStars(int stars)`
- 將值限制在 [0, 3] 範圍
- 在任何持久化或 API 回傳前應用

### 依賴項
- [`nlohmann/json` v3.12.0](https://github.com/nlohmann/json)：JSON 庫
- [`Level/Level.hpp`](../Level/Level.md)：`LevelResultData`、`IsWithinTimeLimit()`、`IsWithinStrokeLimit()`
- C++ 標準庫：`<filesystem>`、`<fstream>`、`<unordered_map>`、`<stdexcept>`
- PTSD Logger：`Util/Logger.hpp` 用於 LOG_WARN、LOG_INFO、LOG_ERROR

## 測試考量

1. **首次執行**：無存檔目錄或 JSON → 應自動產生
2. **升級**：新的最佳星星 → 應持久化並於下次階段載入
3. **降級嘗試**：同關卡較差成績 → 不應覆寫
4. **損毀**：手動編輯 JSON 為無效格式 → 應回退且不崩潰
5. **多關卡**：登記第 1~3 關，以各種順序完成 → 應正確聚合總數

## 未來增強
1. **ProgressMigration**：處理架構升等 (v1 → v2)
2. **ProgressService**：高階管理器用於統計快取、解鎖邏輯
3. **雲端同步**：擴充 `Save()/Load()` 以支援網路持久化
4. **重播系統**：存儲關卡完成中繼資料（時間戳、嘗試次數）
