# UIElement 工廠函式

**標頭檔**：[`include/Screen/UIElement.hpp`](../../include/Screen/UIElement.hpp)

命名空間：`UI::Element`

提供快速建立常用 UI 元素的工廠函式。

```cpp
// 建立背景圖物件
std::shared_ptr<Util::GameObject> Background(const std::string& path);

// 建立按鈕
std::shared_ptr<UI::Button> Button(
    const std::string& path,
    std::function<void()> OnClickHandler
);

// 建立文字物件
std::shared_ptr<Util::GameObject> Text(
    const std::string& text,
    int size,
    const glm::vec2& position,
    const Util::Color& color,
    float z = 1.0f,
    const std::string& fontPath = "Resources/Fonts/Inter.ttf"
);

// 建立圖片物件
std::shared_ptr<Util::GameObject> Image(
    const std::string& path,
    const glm::vec2& position,
    const glm::vec2& scale,
    float z = 1.0f
);
```
