# ResultScreen

**標頭檔**：[`include/Screen/ResultScreen.hpp`](../../include/Screen/ResultScreen.hpp)

繼承 [`UIScreen`](UIScreen.md)。關卡結果畫面，顯示星星評分與截圖。

```cpp
explicit ResultScreen(LevelResult resultData);
ScreenType Update() override;
ScreenType GetScreenType() const override;  // 回傳 ScreenType::RESULT
LevelId GetNextLevelId() const;             // 取得下一關 ID
```
