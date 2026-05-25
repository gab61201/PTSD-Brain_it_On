# LevelConfig

**標頭檔**：[`include/Level/LevelData.hpp`](../../include/Level/LevelData.hpp)

```cpp
struct LevelConfig {
    float timeout = 30.0F;          // 超時秒數
    int strokeLimit = 3;            // 筆畫上限
    std::string targetText = "";    // 提示文字
    std::shared_ptr<GameWorld::PhysicalWorld> world;   // 物理世界
    std::shared_ptr<PassCondition> passCondition;       // 過關條件
};
```
