# LevelData 全域函式

**標頭檔**：[`include/Level/LevelData.hpp`](../../include/Level/LevelData.hpp)

```cpp
// 取得關卡註冊表（內部使用）
std::map<LevelId, LevelFunction>& GetLevelRegistry();

// 取得指定關卡的設定
LevelConfig GetLevelConfig(LevelId id);

// 註冊一個關卡（通常在關卡定義檔的靜態初始化中呼叫）
void RegisterLevel(LevelId id, LevelFunction function);
```

**型別別名**：

```cpp
using LevelFunction = std::function<LevelConfig()>;
```
