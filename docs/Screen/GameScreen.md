# GameScreen

**標頭檔**：[`include/Screen/GameScreen.hpp`](../../include/Screen/GameScreen.hpp)

繼承 [`UIScreen`](UIScreen.md)。遊戲進行中畫面，持有 `Level` 實例。

```cpp
explicit GameScreen(LevelId levelId);
ScreenType Update() override;
ScreenType GetScreenType() const override;     // 回傳 ScreenType::GAME
const LevelResult& GetLastResult() const;      // 取得關卡結果
bool IsNewRecord() const;                      // 是否為新紀錄
```
