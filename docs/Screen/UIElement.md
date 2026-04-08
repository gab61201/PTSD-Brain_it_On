# UIElement

**檔案**: `include/Screen/UIElement.hpp`

UI 元件工具集合，提供快速建立常用按鈕與背景的方法。

## 描述

`UI::Element` 命名空間包含一系列靜態工具函式，用於快速建立常見的 UI 元素，如方形按鈕、圓形按鈕與背景物件。

## 命名空間：`UI::Element`

### `std::shared_ptr<Util::GameObject> Background(const std::string& path)`

建立背景物件。

**參數**:
- `path`: 圖片資源路徑

**回傳值**: 背景 GameObject 指標

### `std::shared_ptr<UI::Button> SquareButton(
    std::function<void()> OnClickHandler,
    const std::string& path = "Resources/Images/Btn_MainButton_Gray.png"
)`

建立方形按鈕。

**參數**:
- `OnClickHandler`: 點擊事件處理函式
- `path`: 圖片資源路徑 (預設為灰色方塊按鈕)

**回傳值**: Button 指標

### `std::shared_ptr<UI::Button> CircleButton(
    std::function<void()> OnClickHandler,
    const std::string& path = "Resources/Images/Btn_OtherButton_Circle01.png"
)`

建立圓形按鈕。

**參數**:
- `OnClickHandler`: 點擊事件處理函式
- `path`: 圖片資源路徑 (預設為圓形按鈕)

**回傳值**: Button 指標

## 使用範例

```cpp
// 建立方形按鈕並綁定點擊事件
auto button = UI::Element::SquareButton([]() {
    std::cout << "方形按鈕被點擊!" << std::endl;
});

// 建立背景圖片
auto background = UI::Element::Background("Resources/Images/Background.png");
```

## 相關類別

- **UI::Button**: 可點擊的按鈕元件
- **Util::GameObject**: 遊戲物件基類
- **UIScreen**: 使用這些元件構建畫面
