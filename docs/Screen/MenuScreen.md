# MenuScreen

**檔案**: `include/Screen/MenuScreen.hpp`

主選單畫面，提供遊戲的主要導航功能。

## 描述

`MenuScreen` 是應用程式啟動後顯示的主選單。它包含進入大廳、設定與離開遊戲等選項。

## 建構式

```cpp
explicit MenuScreen(LevelId* levelId);
```

**參數**:
- `levelId`: 指向當前關卡 ID 的指標，用於儲存玩家選擇的關卡

## 方法

### `void Update() override`

更新選單畫面狀態。每幀執行以處理輸入與按鈕狀態。

**覆寫自**: `UIScreen::Update()`

### `ScreenType GetNextScreenType() override`

取得下一個要切換的畫面類型。依據玩家選擇：
- 進入遊戲 → LobbyScreen
- 設定 → SettingsScreen
- 離開 → App 結束

**回傳值**: `UI::ScreenType` 列舉值

**覆寫自**: `UIScreen::GetNextScreenType()`

### `ScreenType GetScreenType() const override`

取得當前畫面的類型識別。

**回傳值**: `UI::ScreenType::MENU`

**覆寫自**: `UIScreen::GetScreenType()`

## 成員變數

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_LevelId` | `LevelId*` | 指向關卡 ID 的指標，用於儲存玩家選擇 |

## 繼承關係

- **繼承自**: `UI::UIScreen` (抽象基類)
- **實現方法**: 
  - `Update()`
  - `GetNextScreenType()`
  - `GetScreenType()`

## 相關類別

- **UIScreen**: 抽象基類，定義所有畫面的共同介面
- **LobbyScreen**: 大廳畫面，供玩家選擇關卡
- **SettingsScreen**: 設定畫面
- **UIElement**: UI 元件工具集合
