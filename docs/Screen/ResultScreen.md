# ResultScreen

**標頭檔**：[`include/Screen/ResultScreen.hpp`](../../include/Screen/ResultScreen.hpp)

繼承 [`UIScreen`](UIScreen.md)。關卡結果畫面，顯示星星評分與截圖。

按鈕行為：`Retry` 會重玩目前關卡，`Next` 會進入下一關；若已經是最後一關，`Next` 會回到選單。

```cpp
explicit ResultScreen(LevelResult resultData);
ScreenType Update() override;
ScreenType GetScreenType() const override;  // 回傳 ScreenType::RESULT
LevelId GetNextLevelId() const;             // 取得下一關 ID
```
