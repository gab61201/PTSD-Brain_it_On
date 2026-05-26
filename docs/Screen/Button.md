# Button

**標頭檔**：[`include/Screen/Button.hpp`](../../include/Screen/Button.hpp)

繼承 `Util::GameObject`。可點擊的 UI 按鈕。

```cpp
Button(
    const std::shared_ptr<Core::Drawable>& drawable,
    float zIndex,
    const glm::vec2& pivot = {0, 0},
    bool visible = true
);
```

#### 方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `Update` | `void Update()` | 每幀更新，檢測點擊 |
| `OnClick` | `void OnClick(std::function<void()> handler)` | 設定點擊回呼函式 |
