# ProgressRecord

**標頭檔**：[`include/Util/ProgressStore.hpp`](../../include/Util/ProgressStore.hpp)

```cpp
struct ProgressRecord {
    std::string screenshotFilename;          // 截圖檔名
    std::array<bool, 3> stars{false, false, false}; // [通關, 時間內, 筆畫內]
    float remainingTime = 0.0f;              // 最佳剩餘時間
    int usedStrokes = 0;                     // 最佳筆畫數
};
```
