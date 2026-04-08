# Button

**檔案**: `include/Screen/Button.hpp`

UI 按鈕元件，提供使用者互動介面。

## 描述

`Button` 繼承自 `GameObject`，是一個可點擊的 UI 元素。當游標指向按鈕並按下時，會觸發預設的事件處理函式。

## 建構式

```cpp
Button(
    const std::shared_ptr<Core::Drawable>& drawable,  // 可繪製物件 (如圖片或文字)
    const float zIndex,                               // 渲染順序 (z-index)
    const glm::vec2& pivot = {0, 0},                  // 旋轉軸心點 (預設中心)
    const bool visible = true                         // 是否可見 (預設為 true)
);
```

**參數說明**:
- `drawable`: 按鈕的視覺表示，可以是圖片、文字或其他可繪製物件
- `zIndex`: 渲染順序，值越大越先顯示在上方
- `pivot`: 旋轉軸心點座標，用於定義旋轉基準
- `visible`: 控制按鈕是否顯示

## 方法

### `void Update()`

更新按鈕狀態。每幀執行以檢查游標位置與輸入狀態。

### `void OnClick(std::function<void()> OnClickHandler)`

設定按鈕點擊事件處理函式。

**參數**:
- `OnClickHandler`: 當按鈕被點擊時執行的 Lambda 或函式指標

```cpp
// 使用範例
auto button = std::make_shared<Button>(drawable, zIndex);
button->OnClick([]() {
    std::cout << "按鈕被點擊了!" << std::endl;
});
```

## 私有方法

### `bool IsCursorPointing()`

檢查游標是否指向此按鈕。用於判斷當前滑鼠位置是否在按鈕範圍內。

**回傳值**: 
- `true`: 游標在按鈕上
- `false`: 游標不在按鈕上

### `void OnPressing()`

處理按鈕被按下的邏輯。當檢測到點擊動作時呼叫。

## 繼承關係

- **繼承自**: `Util::GameObject` (Core 框架的遊戲物件基類)

## 相關類別

- **UIElement::SquareButton**: 建立方形按鈕的工具函式
- **UIElement::CircleButton**: 建立圓形按鈕的工具函式
- **UIScreen**: 使用 Button 來構建畫面
