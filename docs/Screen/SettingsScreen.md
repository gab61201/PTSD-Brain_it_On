# SettingsScreen

**檔案**: `include/Screen/SettingsScreen.hpp`

設定畫面，提供遊戲選項調整功能。

## 描述

`SettingsScreen` 是玩家的設定介面，允許調整遊戲相關參數如音量、難度等 (具體設定項目視實作而定)。

## 建構式

```cpp
SettingsScreen();
```

無參數建構式，建立設定畫面的基本元件。

## 方法

### `void Update() override`

更新設定畫面狀態。每幀執行以處理輸入與設定變更。

**覆寫自**: `UIScreen::Update()`

### `ScreenType GetNextScreenType() override`

取得下一個要切換的畫面類型。通常為返回主選單或大廳。

**回傳值**: `UI::ScreenType` 列舉值

**覆寫自**: `UIScreen::GetNextScreenType()`

### `ScreenType GetScreenType() const override`

取得當前畫面的類型識別。

**回傳值**: `UI::ScreenType::SETTINGS`

**覆寫自**: `UIScreen::GetScreenType()`

## 繼承關係

- **繼承自**: `UI::UIScreen` (抽象基類)
- **實現方法**: 
  - `Update()`
  - `GetNextScreenType()`
  - `GetScreenType()`

## 相關類別

- **UIScreen**: 抽象基類，定義所有畫面的共同介面
- **MenuScreen**: 主選單畫面
- **LobbyScreen**: 大廳畫面
- **GameScreen**: 遊戲進行畫面
- **UIElement**: UI 元件工具集合
