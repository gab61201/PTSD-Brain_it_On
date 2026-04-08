# LobbyScreen

**檔案**: `include/Screen/LobbyScreen.hpp`

大廳畫面，提供關卡選擇介面。

## 描述

`LobbyScreen` 是玩家在進入遊戲後首先看到的畫面。它顯示所有已解鎖的關卡供玩家選擇，並允許玩家返回主選單或進入設定。

## 建構式

```cpp
LobbyScreen();
```

無參數建構式，建立大廳畫面的基本元件。

## 方法

### `void Update() override`

更新大廳畫面狀態。每幀執行以處理輸入與狀態變化。

**覆寫自**: `UIScreen::Update()`

### `ScreenType GetNextScreenType() override`

取得下一個要切換的畫面類型。依據玩家的選擇決定：
- 選擇關卡 → GameScreen
- 返回主選單 → MenuScreen
- 進入設定 → SettingsScreen

**回傳值**: `UI::ScreenType` 列舉值

**覆寫自**: `UIScreen::GetNextScreenType()`

### `ScreenType GetScreenType() const override`

取得當前畫面的類型識別。

**回傳值**: `UI::ScreenType::LOBBY`

**覆寫自**: `UIScreen::GetScreenType()`

## 相關類別

- **UIScreen**: 抽象基類，定義所有畫面的共同介面
- **MenuScreen**: 主選單畫面
- **SettingsScreen**: 設定畫面
- **GameScreen**: 遊戲進行畫面
- **UIElement**: UI 元件工具集合
