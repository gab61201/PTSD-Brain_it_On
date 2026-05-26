# App

**標頭檔**：[`include/App.hpp`](../include/App.hpp)

應用程式主控制器，管理遊戲生命週期與畫面切換。

### 列舉

#### `App::State`

```cpp
enum class State {
    START,   // 初始化階段
    UPDATE,  // 主更新迴圈
    END,     // 結束清理
};
```

### 方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `GetCurrentState` | `State GetCurrentState() const` | 取得目前應用程式狀態 |
| `Start` | `void Start()` | 初始化：載入存檔、播放 BGM、建立 LobbyScreen |
| `Update` | `void Update()` | 每幀呼叫，驅動畫面更新與切換邏輯 |
| `End` | `void End()` | 結束清理（預留） |

### 成員變數（private）

| 變數 | 型別 | 說明 |
|------|------|------|
| `m_CurrentState` | `State` | 當前應用程式狀態 |
| `m_Screen` | `std::unique_ptr<UI::UIScreen>` | 當前活躍的 UI 畫面 |
| `m_SelectedLevelId` | `LevelId` | 玩家選擇的關卡 ID |
