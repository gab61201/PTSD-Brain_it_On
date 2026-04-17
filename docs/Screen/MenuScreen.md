# MenuScreen

## 概述
MenuScreen 顯示可用關卡的動態列表，可追蹤玩家進度。玩家可選擇關卡開始遊戲或返回大廳。

## 架構

### 資料流：結算星星 → 進度存儲 → 選單顯示
```
GameScreen (遊戲中)
    ↓ (關卡完成)
ResultScreen (顯示三個獨立星星條件)
    ↓
App::Update() RESULT 轉場
    ├─ 計算星星：ProgressStore::CalculateStars(resultData)
    │   (通過 ? 1 : 0) + (時間達成 ? 1 : 0) + (筆數達成 ? 1 : 0)
    ├─ UpdateBestStars(levelId, stars) [只升不降]
    └─ Save() → Resources/Save/progress.json
    ↓
MenuScreen (下次進入時重新整理)
    ├─ 載入 ProgressStore 各關最佳星星
    ├─ 顯示總星星數：X（所有最佳星星之和）
    └─ 列出各關，含該關最佳星星 (S/3)
```

### 星星計算規則
- **三個獨立條件**（不是連續的）：
  1. `passed`：通過關卡條件
  2. `IsWithinTimeLimit()`：在目標時間內解決
  3. `IsWithinStrokeLimit()`：在目標筆數內解決
- **非連續組合允許**：例如只達成星星 {1, 3} 而無 {2}
- **最佳星星策略**：只在新結果超過舊紀錄時持久化
- **範圍**：每關 0~3 顆星

## UI 排版

### 頂部區域
- **總星星數顯示**：文字位置 (0, 300)
  - 格式：`"總星星數：X"`，X = 所有最佳星星總和
  - 字體：Inter.ttf，大小 48，白色 (RGB 255,255,255)

### 中間區域
- **動態關卡清單**：垂直排列，起點 y=200，每列間距 y=-80
  - 每個關卡項目包含：
    1. **關卡按鈕**：SquareButton 位置 (0, y)，圖片 "Btn_MainButton_Gray.png"
       - 點擊時：設定 `m_LevelId` 並轉場到 GAME
    2. **關卡標籤**：文字位置 (-100, y)
       - 格式：`"第 N 關"`，N = levelId + 1
       - 字體：Inter.ttf，大小 32，白色 (RGB 255,255,255)
    3. **星星標籤**：文字位置 (+100, y)
       - 格式：`"★ S/3"`，S = 該關最佳星星
       - 字體：Inter.ttf，大小 32，金色 (RGB 255,215,0)

### 底部區域
- **返回按鈕**：CircleButton 位置 (-560, -300)
  - 圖片：Resources/Images/Btn_Back.png
  - 點擊時：轉場回 LOBBY

## 實作細節

### 建構子：`MenuScreen(LevelId* levelId, ProgressStore* progressStore)`
- **參數**：
  - `levelId`：指向已選關卡 ID（按鈕點擊時設定）
  - `progressStore`：指向全域進度追蹤器（用於讀取最佳星星）
- **初始化流程**：
  1. 新增背景圖片
  2. 建立並渲染總星星數文字
  3. 建立並註冊返回按鈕
  4. 若 `progressStore` 為空，提前返回（防護）
  5. 取得 LevelRegistry，提取所有註冊關卡 ID，按數值升序排序
  6. 對每個關卡，建立按鈕組（按鈕 + 標籤文字）並新增到渲染器

### 動態關卡註冊表整合
- 來源：`GetLevelRegistry()` 自 `Level/LevelData.hpp`
- 關卡按 ID 升序排列以確保順序一致
- 目前只有第 1 關已註冊
- 整合點：若稍後註冊第 2、3 關，會自動出現

### 按鈕點擊行為
- **關卡按鈕**：`[this, levelId]() { *m_LevelId = levelId; m_NextScreenType = ScreenType::GAME; }`
- **返回按鈕**：`[this]() { m_NextScreenType = ScreenType::LOBBY; }`

## 依賴項
- [ProgressStore](../Progress/ProgressStore.md)：提供 `GetBestStars()`、`GetTotalStars()`
- [LevelData](../Level/LevelData.md)：提供 `GetLevelRegistry()` 用於動態列舉關卡
- [UIElement](UIElement.md)：提供按鈕工廠 (`SquareButton`、`CircleButton`)
- PTSD 框架：Util::Text、Util::GameObject、Util::Renderer、Util::Color

## 未來增强
1. 關卡預覽圖/縮圖
2. 大量關卡時的捲軸區域
3. 解鎖系統（灰顯鎖定關卡）
4. 遊玩時間／最佳時間顯示
5. 關卡難度指示器
