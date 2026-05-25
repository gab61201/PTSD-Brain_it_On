# LevelResult

**標頭檔**：[`include/Level/LevelData.hpp`](../../include/Level/LevelData.hpp)

```cpp
struct LevelResult {
    LevelId levelId = LevelId::LEVEL_1;
    bool passed = false;           // 是否通關
    float goalTime = 0.0f;         // 目標時間
    float solvedTime = 0.0f;       // 實際花費時間
    int goalStroke = 0;            // 目標筆畫數
    int usedStroke = 0;            // 實際筆畫數
    std::string screenshotFilename; // 截圖檔名
};
```
