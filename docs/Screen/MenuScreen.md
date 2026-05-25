# MenuScreen

**標頭檔**：[`include/Screen/MenuScreen.hpp`](../../include/Screen/MenuScreen.hpp)

繼承 [`UIScreen`](UIScreen.md)。關卡選擇畫面。

```cpp
MenuScreen();
ScreenType Update() override;
ScreenType GetScreenType() const override;  // 回傳 ScreenType::MENU
LevelId GetSelectedLevelId() const;          // 取得玩家選擇的關卡
```
