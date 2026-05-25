# LobbyScreen

**標頭檔**：[`include/Screen/LobbyScreen.hpp`](../../include/Screen/LobbyScreen.hpp)

繼承 [`UIScreen`](UIScreen.md)。遊戲大廳畫面（主畫面）。

```cpp
LobbyScreen();
ScreenType Update() override;
ScreenType GetScreenType() const override;  // 回傳 ScreenType::LOBBY
```
