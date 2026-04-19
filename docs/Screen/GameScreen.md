# GameScreen

**檔案**: `include/Screen/GameScreen.hpp`

遊戲進行畫面，負責顯示關卡內容並處理玩家互動。

## 描述

`GameScreen` 是實際進行遊戲的畫面類別。它載入指定的關卡並管理遊戲流程，包括繪製階段、物理模擬與過關判定。

## 建構式

```cpp
explicit GameScreen(LevelId* levelId);
```

**參數**:
- `levelId`: 指向當前要執行的關卡 ID 指標

## 方法

### `void Update() override`

更新遊戲畫面狀態。每幀執行以下操作：
1. 呼叫關卡的 `Update()` 方法
2. 處理玩家繪製輸入
3. 檢查過關結果
4. 準備切換到下一個畫面 (RESULT)

**覆寫自**: `UIScreen::Update()`

### `ScreenType GetNextScreenType() override`

取得下一個要切換的畫面類型。遊戲結束後會返回 RESULT。

**回傳值**: `UI::ScreenType` 列舉值

**覆寫自**: `UIScreen::GetNextScreenType()`

### `ScreenType GetScreenType() const override`

取得當前畫面的類型識別。

**回傳值**: `UI::ScreenType::GAME`

**覆寫自**: `UIScreen::GetScreenType()`

### `bool TryGetResultData(LevelResultData* outResult) const`

嘗試取得關卡結果資料。在畫面切換到 RESULT 時由 App 呼叫，用於傳遞結算資料。

**參數**:
- `outResult`: 輸出參數，指向要填入的 `LevelResultData` 結構體

**回傳值**: 
- `true`: 成功取得結果資料
- `false`: 無法取得 (如關卡尚未結束)

## 成員變數 (私有)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_Level` | `Level` | 當前關卡實體，管理遊戲邏輯 |

## 繼承關係

- **繼承自**: `UI::UIScreen` (抽象基類)
- **實現方法**: 
  - `Update()`
  - `GetNextScreenType()`
  - `GetScreenType()`
  - `TryGetResultData()`

## 相關畫面切換流程

```
GameScreen → 過關/失敗
    ↓
ResultScreen (顯示結算結果)
    ↓
MenuScreen / LobbyScreen
```

## 相關類別

- **UIScreen**: 抽象基類，定義所有畫面的共同介面
- **Level**: 關卡控制器，管理遊戲進行
- **LevelResultData**: 關卡結果資料結構
- **ScreenType**: 畫面類型列舉 (LOBBY, SETTINGS, MENU, GAME, RESULT)
