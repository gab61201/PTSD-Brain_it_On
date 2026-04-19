# ResultScreen

**檔案**: `include/Screen/ResultScreen.hpp`

結果畫面，顯示關卡完成後的統計與星星評級。

## 描述

`ResultScreen` 在玩家完成關卡後顯示，呈現本次遊戲的結果數據，包括是否過關、用時、使用筆數等資訊。

## 建構式

```cpp
ResultScreen(LevelId* levelId, const LevelResultData& resultData);
```

**參數**:
- `levelId`: 指向已選關卡 ID 的指標
- `resultData`: 關卡結果資料，包含通過狀態、用時、筆數等

## 方法

### `void Update() override`

更新結果畫面狀態。每幀執行以處理輸入與狀態變化。

**覆寫自**: `UIScreen::Update()`

### `ScreenType GetNextScreenType() override`

取得下一個要切換的畫面類型。通常返回 MENU (主選單)。

**回傳值**: `UI::ScreenType` 列舉值

**覆寫自**: `UIScreen::GetNextScreenType()`

### `ScreenType GetScreenType() const override`

取得當前畫面的類型識別。

**回傳值**: `UI::ScreenType::RESULT`

**覆寫自**: `UIScreen::GetScreenType()`

## 成員變數 (私有)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_ResultData` | `LevelResultData` | 關卡結果資料 |

## 繼承關係

- **繼承自**: `UI::UIScreen` (抽象基類)
- **實現方法**: 
  - `Update()`
  - `GetNextScreenType()`
  - `GetScreenType()`

## 相關畫面切換流程

```
GameScreen → ResultScreen → MenuScreen
```

## 相關類別

- **UIScreen**: 抽象基類，定義所有畫面的共同介面
- **LevelResultData**: 關卡結果資料結構
- **App**: 在 RESULT 轉場時建立此畫面並傳遞結算資料
