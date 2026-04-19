# App

**檔案**: `include/App.hpp`, `src/App.cpp`

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
1. 載入或建立進度存檔 (`ProgressStore::LoadOrCreateDefault()`)
2. 播放背景音樂 (`BGM("Resources/Audios/BGM.mp3")`)
3. 設定初始畫面為 LOBBY (`LobbyScreen`)
4. 準備進入 UPDATE 狀態

### `void Update()`

更新應用程式狀態。每幀執行以下邏輯：
1. 從當前畫面取得 `GetScreenType()` 與 `GetNextScreenType()`
2. 若兩者不同，根據 `m_ScreenType` 建立新的畫面物件並切換
3. 呼叫 `m_Screen->Update()` 更新當前畫面
4. 檢查退出輸入 (`Util::Input::IfExit()`)，若是則進入 END 狀態

**畫面切換邏輯**:
```cpp
switch (m_ScreenType) {
    case UI::ScreenType::LOBBY:
        m_Screen = std::make_unique<UI::LobbyScreen>();
        break;
    case UI::ScreenType::SETTINGS:
        m_Screen = std::make_unique<UI::SettingsScreen>();
        break;
    case UI::ScreenType::MENU:
        m_Screen = std::make_unique<UI::MenuScreen>(&m_SelectedLevelId, &m_ProgressStore);
        break;
    case UI::ScreenType::GAME:
        m_Screen = std::make_unique<UI::GameScreen>(&m_SelectedLevelId);
        break;
    case UI::ScreenType::RESULT: {
        // 從 GameScreen 取得結算資料，更新進度後儲存
        LevelResultData resultData{};
        if (static_cast<UI::GameScreen*>(m_Screen.get())->TryGetResultData(&resultData)) {
            m_ProgressStore.UpdateBestStars(resultData.levelId, ...);
            m_ProgressStore.Save();
        }
        m_Screen = std::make_unique<UI::ResultScreen>(&m_SelectedLevelId, resultData);
        break;
    }
}
```

### `void End()`

結束應用程式。執行清理工作：
1. 記錄追蹤訊息 (`LOG_TRACE`)

## 成員變數 (私有)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_CurrentState` | `State` | 當前應用程式狀態 (預設 START) |
| `m_ScreenType` | `UI::ScreenType` | 當前畫面類型 |
| `m_Screen` | `std::unique_ptr<UI::UIScreen>` | 當前畫面的指標 |
| `m_SelectedLevelId` | `LevelId` | 玩家選擇的關卡 ID |
| `m_ProgressStore` | `ProgressStore` | 進度存檔管理器 |

## 相關類別

- **UIScreen**: 抽象基類，定義所有畫面的共同介面
- **LobbyScreen**: 大廳畫面 (初始畫面)
- **MenuScreen**: 主選單畫面
- **GameScreen**: 遊戲進行畫面
- **ResultScreen**: 結果畫面
- **SettingsScreen**: 設定畫面
- **Level**: 關卡控制器
- **ProgressStore**: 進度存檔管理

## App 生命週期

```
1. Start()
   → LoadOrCreateDefault()
   → BGM.Play()
   → m_Screen = LobbyScreen
   ↓
2. UPDATE 狀態，每幀執行 Update()
   → 檢查畫面切換 (GetNextScreenType != GetScreenType)
   → 建立新畫面並替換 m_Screen
   → m_Screen->Update()
   → 檢查退出輸入
   ↓
3. End()
   → LOG_TRACE("End")
   ↓
4. 程式結束
```

## 相關檔案

- `src/App.cpp`: App 類別的實作檔案
- `include/Screen/UIScreen.hpp`: 畫面基類
- `include/Progress/ProgressStore.hpp`: 進度存檔管理
