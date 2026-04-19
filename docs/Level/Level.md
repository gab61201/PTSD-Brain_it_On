# Level

**檔案**: `include/Level/Level.hpp`, `src/Level/Level.cpp`

關卡控制器，管理單一關卡的完整生命週期與遊戲狀態。

## 描述

`Level` 類別負責控制單一關卡的執行流程，包括等待開始、繪製階段、正式遊玩以及完成後的處理。它整合了物理世界、HUD 介面與過關條件檢測。

## LevelId 列舉

定義了可用的關卡 ID：

| 值 | 說明 |
|----|------|
| `LEVEL_1` | 第一關 |
| `LEVEL_2` | 第二關 |
| `LEVEL_3` | 第三關 |
| `LEVEL_4` | 第四關 |
| `LEVEL_5` | 第五關 |

## State 列舉

定義了關卡的內部狀態：

| 值 | 說明 |
|----|------|
| `WAITING` | 等待玩家開始 |
| `DRAWING` | 玩家可以繪製物件的階段 |
| `PLAYING` | 正式遊玩，物理引擎啟動 |
| `FINISHED` | 關卡結束 (過關或失敗) |

## LevelResultData 結構

```cpp
struct LevelResultData {
    LevelId levelId = LevelId::LEVEL_1;
    bool passed = false;
    float goalTime = 0.0f;
    float solvedTime = 0.0f;
    int goalStroke = 0;
    int usedStroke = 0;
};
```

| 名稱 | 類型 | 說明 |
|------|------|------|
| `levelId` | `LevelId` | 關卡 ID |
| `passed` | `bool` | 是否過關 |
| `goalTime` | `float` | 目標時間限制 |
| `solvedTime` | `float` | 實際用時 |
| `goalStroke` | `int` | 目標筆劃限制 |
| `usedStroke` | `int` | 實際使用筆數 |

## 建構式

```cpp
Level(LevelId levelId);
```

**參數**:
- `levelId`: 要載入的關卡 ID

## 方法

### `void Reset()`

重置關卡狀態，重新開始此關卡。會恢復初始物件位置並重設計時器。如果關卡有設定筆劃限制，HUD 也會一併重置成新的 `remaining/total` 顯示。

### `void Update()`

更新畫面與遊戲邏輯。每幀執行以下操作：
1. 根據當前狀態呼叫對應的狀態處理函式 (`Waiting`, `Drawing`, `Playing`, `Finished`)
2. 更新物理世界 (`m_World->Update()`)
3. 檢查過關條件 (`m_PassCondition->Check(...)`)
4. 更新 HUD 顯示 (`m_HUD->UpdateTimer()`, `m_HUD->UpdateStrokeLimit()`)

### `float GetRemainingTime() const`

取得剩餘時間。

**回傳值**: `max(0.0f, m_Timeout - m_Time)`，大於 0 表示剩餘秒數，等於 0 表示時間耗盡

### `LevelResultData GetResultData() const`

取得關卡結果資料，用於結果畫面顯示與進度更新。

**回傳值**: `LevelResultData` 結構體

### `LevelId GetLevelId() const`

取得當前關卡 ID。

**回傳值**: `LevelId`

### `State GetState() const`

取得當前關卡狀態。

**回傳值**: 當前狀態 (`WAITING`, `DRAWING`, `PLAYING`, `FINISHED`)

## 狀態處理函式 (私有)

| 方法 | 說明 |
|------|------|
| `Waiting()` | 等待玩家確認開始的階段 |
| `Drawing()` | 玩家可以繪製物理物件的階段 |
| `Playing()` | 正式遊玩，物件受物理引擎控制 |
| `Finished()` | 關卡結束後的處理與過關判定 |

## 全域函式

### `bool IsWithinTimeLimit(const LevelResultData& resultData)`

檢查是否在時間內完成。

**判斷**: `solvedTime <= goalTime`

### `bool IsWithinStrokeLimit(const LevelResultData& resultData)`

檢查是否在筆劃限制內完成。

**判斷**: `usedStroke <= goalStroke`

## 成員變數 (私有)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_LevelId` | `LevelId` | 關卡 ID |
| `m_state` | `State` | 當前狀態 (預設 WAITING) |
| `m_Time` | `float` | 遊戲進行時間 (秒) |
| `m_Timeout` | `float` | 遊戲限制時間 (秒) |
| `m_StrokeLimit` | `int` | 當前關卡的筆劃限制 |
| `m_World` | `std::shared_ptr<GameWorld::PhysicalWorld>` | 物理世界實體 |
| `m_PassCondition` | `std::shared_ptr<PassCondition>` | 過關條件 |
| `m_HUD` | `std::unique_ptr<LevelHUD>` | HUD 介面物件 |

## 相關類別

- **LevelData**: 關卡配置資料，包含時間限制、筆劃限制與目標文字
- **PhysicalWorld**: 物理世界管理員，處理所有物理模擬
- **PassCondition**: 過關條件基類
- **LevelHUD**: 畫面上方顯示的時間、提示與筆劃限制
