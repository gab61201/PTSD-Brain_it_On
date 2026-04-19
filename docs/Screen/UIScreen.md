# UIScreen

**檔案**: `include/Screen/UIScreen.hpp`

抽象基類，定義所有畫面的共同介面。

## 描述

`UIScreen` 是所有遊戲畫面的抽象基類。每個具體畫面 (如主選單、大廳、設定、遊戲) 都必須繼承此類別並實現其純虛函式。

## 常數

| 名稱 | 值 | 說明 |
|------|-----|------|
| `RESOLUTION_X` | `1280.0F` | 畫面寬度解析度 |
| `RESOLUTION_Y` | `720.0F` | 畫面高度解析度 |

## ScreenType 列舉

定義了所有可能的畫面類型：

| 值 | 說明 |
|----|------|
| `LOBBY` | 大廳畫面，供玩家選擇關卡 |
| `SETTINGS` | 設定畫面 |
| `MENU` | 主選單畫面 |
| `GAME` | 遊戲進行畫面 |
| `RESULT` | 結果畫面，顯示關卡完成後的星星與統計 |

## 建構式

```cpp
UIScreen() = default;
virtual ~UIScreen() = default;
```

預設建構式與虛擬解構式。

## 方法

### `virtual void Update() = 0`

更新畫面狀態。每幀執行以處理輸入、邏輯與渲染準備。

**純虛函式**: 子類別必須實作此方法

### `virtual ScreenType GetNextScreenType() = 0`

取得下一個要切換的畫面類型。用於處理畫面之間的轉換邏輯。

**純虛函式**: 子類別必須實作

**回傳值**: 下一畫面類型 (`LOBBY`, `SETTINGS`, `MENU`, `GAME`, `RESULT`)

### `virtual ScreenType GetScreenType() const = 0`

取得當前畫面的類型識別。用於 App 判斷目前處於哪個畫面。

**純虛函式**: 子類別必須實作

**回傳值**: 當前畫面類型 (`LOBBY`, `SETTINGS`, `MENU`, `GAME`, `RESULT`)

## 成員變數 (受保護)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_Renderer` | `Util::Renderer` | 渲染器，用於繪製畫面元素 |
| `m_NextScreenType` | `ScreenType` | 下一個要切換的畫面類型 |
| `m_Buttons` | `std::vector<std::shared_ptr<UI::Button>>` | 按鈕列表，包含所有可互動按鈕 |

## 繼承關係

- **抽象基類**: `UIScreen`
- **直接子類別**:
  - `MenuScreen`: 主選單
  - `LobbyScreen`: 大廳
  - `SettingsScreen`: 設定
  - `GameScreen`: 遊戲進行
  - `ResultScreen`: 結果畫面

## App 整合流程

```cpp
// App::Update() 中的使用方式
void Update() {
    const ScreenType currentScreenType = m_Screen->GetScreenType();
    m_ScreenType = m_Screen->GetNextScreenType();
    if (m_ScreenType != currentScreenType) {
        switch (m_ScreenType) {
            case UI::ScreenType::LOBBY:
                m_Screen = std::make_unique<UI::LobbyScreen>();
                break;
            // ... 其他畫面切換邏輯
        }
    }
    m_Screen->Update();
}
```

## 相關類別

- **MenuScreen**: 主選單實現
- **LobbyScreen**: 大廳實現
- **SettingsScreen**: 設定實現
- **GameScreen**: 遊戲進行實現
- **ResultScreen**: 結果實現
- **UI::Button**: 按鈕元件
- **UI::Element**: UI 工具集合
