# UIScreen

抽象基底類別，所有畫面的父類。

**標頭檔**：[`include/Screen/UIScreen.hpp`](../../include/Screen/UIScreen.hpp)

#### 純虛方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `Update` | `virtual ScreenType Update() = 0` | 更新畫面並回傳下一個畫面類型 |
| `GetScreenType` | `virtual ScreenType GetScreenType() const = 0` | 取得當前畫面類型 |

#### Protected 成員

| 變數 | 型別 | 說明 |
|------|------|------|
| `m_Renderer` | `Util::Renderer` | 渲染器 |
| `m_NextScreenType` | `ScreenType` | 下一個畫面類型 |
| `m_Buttons` | `vector<shared_ptr<UI::Button>>` | 按鈕列表 |
