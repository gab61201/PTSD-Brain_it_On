# Level

**標頭檔**：[`include/Level/Level.hpp`](../../include/Level/Level.hpp)

關卡控制器，管理關卡狀態機、物理世界與過關判定。

```cpp
explicit Level(LevelId levelId);
```

#### 關卡狀態

```cpp
enum class State {
    WAITING,   // 等待玩家開始
    DRAWING,   // 玩家繪製中
    PLAYING,   // 物理模擬進行中
    FINISHED   // 關卡結束（通過或超時）
};
```

#### 公開方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `Reset` | `void Reset()` | 重設關卡到初始狀態 |
| `Update` | `void Update()` | 每幀更新（根據當前狀態分派） |
| `GetRemainingTime` | `float GetRemainingTime() const` | 取得剩餘時間（秒），最小 0 |
| `GetLevelId` | `LevelId GetLevelId() const` | 取得關卡 ID |
| `GetState` | `State GetState() const` | 取得當前狀態 |
| `GetLastResult` | `const LevelResult& GetLastResult() const` | 取得最近結果 |
| `IsNewRecord` | `bool IsNewRecord() const` | 是否為新紀錄 |
| `Save` | `void Save()` | 儲存關卡結果 |
