# App

**檔案**: `include/App.hpp`

應用程式核心，管理整個遊戲的生命週期與狀態機。

## 描述

`App` 類別是整個 PTSD Brain It On! 應用程式的控制中心。它負責管理應用程式的狀態轉換 (START → UPDATE → END) 以及畫面切換邏輯。

## State 列舉

定義了應用程式的三個主要狀態：

| 值 | 說明 |
|----|------|
| `START` | 初始化階段，載入資源與建立初始物件 |
| `UPDATE` | 更新階段，每幀執行遊戲邏輯與渲染 |
| `END` | 結束階段，清理資源並準備退出 |

## 方法

### `State GetCurrentState() const`

取得當前應用程式狀態。

**回傳值**: 當前狀態 (`START`, `UPDATE`, `END`)

### `void Start()`

啟動應用程式。執行以下操作：
1. 初始化 PTSD 框架
2. 建立初始畫面 (主選單)
3. 準備進入 UPDATE 狀態

### `void Update()`

更新應用程式狀態。每幀執行以下邏輯：
1. 根據當前畫面類型呼叫對應畫面的 Update 方法
2. 處理畫面切換邏輯
3. 更新關卡狀態

**流程**:
```cpp
switch (m_ScreenType) {
    case ScreenType::LOBBY:
        m_Screen->Update();
        // 檢查是否需要切換到 MENU、SETTINGS 或 GAME
        break;
    case ScreenType::MENU:
        static_cast<MenuScreen*>(m_Screen.get())->Update();
        break;
    // ... 其他畫面
}
```

### `void End()`

結束應用程式。執行清理工作：
1. 釋放所有資源
2. 關閉框架
3. 準備退出程式

## 成員變數 (私有)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_CurrentState` | `State` | 當前應用程式狀態 (預設 START) |
| `m_ScreenType` | `Screen::ScreenType` | 當前畫面類型 |
| `m_Screen` | `std::unique_ptr<UIScreen>` | 當前畫面的指標 |
| `m_SelectedLevelId` | `LevelId` | 玩家選擇的關卡 ID |

## 相關類別

- **UIScreen**: 抽象基類，定義所有畫面的共同介面
- **MenuScreen**: 主選單畫面
- **LobbyScreen**: 大廳畫面
- **SettingsScreen**: 設定畫面
- **GameScreen**: 遊戲進行畫面
- **Level**: 關卡控制器

## App 生命週期

```
1. Start()
   ↓
2. UPDATE 狀態，每幀執行 Update()
   → m_Screen->Update()
   → 處理畫面切換
   ↓
3. End()
   ↓
4. 程式結束
```

## 相關檔案

- `src/App.cpp`: App 類別的實作檔案
- `include/Screen/UIScreen.hpp`: 畫面基類
