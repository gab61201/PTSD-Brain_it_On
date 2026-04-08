# LevelData

**檔案**: `include/Level/LevelData.hpp`

關卡配置資料結構，定義了每個關卡的設定與資源。

## 描述

`LevelData` 是一個結構體，用於儲存單一關卡的所有配置資訊，包括時間限制、目標提示文字以及物理世界實體。

## LevelId 列舉

定義了可用的關卡 ID：

| 值 | 說明 |
|----|------|
| `LEVEL_1` | 第一關 |
| `LEVEL_2` | 第二關 |
| `LEVEL_3` | 第三關 |
| `LEVEL_4` | 第四關 |
| `LEVEL_5` | 第五關 |

## LevelData 結構

```cpp
struct LevelData {
    float timeout = 30.0F;                      // 時間限制 (秒)
    std::string targetText = "";                // 目標提示文字
    std::shared_ptr<GameWorld::PhysicalWorld> world;  // 物理世界實體
};
```

### 成員變數

| 名稱 | 類型 | 預設值 | 說明 |
|------|------|--------|------|
| `timeout` | `float` | `30.0F` | 關卡時間限制，玩家需在時間內達成目標 |
| `targetText` | `std::string` | `""` | 顯示給玩家的目標提示 (如：「將球放入盒子」) |
| `world` | `std::shared_ptr<PhysicalWorld>` | `nullptr` | 關卡的物理世界實體，包含所有預先放置的物件 |

## LevelFunction 類型定義

```cpp
using LevelFunction = std::function<LevelData()>;
```

用於註冊關卡建立函式的函式指標類型。返回一個 `LevelData` 結構體。

## API 函式

### `std::unordered_map<LevelId, LevelFunction>& GetLevelRegistry()`

取得全域的關卡註冊表。此函式使用 `std::function` 包裝避免靜態初始化順序問題。

**回傳值**: 關卡註冊映射表 (LevelId → LevelFunction)

### `LevelData GetLevelData(LevelId id)`

取得指定關卡的配置資料。

**參數**:
- `id`: 關卡 ID

**回傳值**: 該關卡的 LevelData 結構體

### `void RegisterLevel(LevelId id, LevelFunction function)`

註冊一個新的關卡。用於將關卡建立函式與關卡 ID 綁定。

**參數**:
- `id`: 關卡 ID
- `function`: 返回 LevelData 的函式指標

## 使用範例

```cpp
// 註冊關卡 1
RegisterLevel(LevelId::LEVEL_1, []() {
    LevelData data;
    data.timeout = 30.0F;
    data.targetText = "將球放入盒子";
    // ... 建立物理世界並指派給 data.world
    return data;
});

// 取得關卡資料
LevelData level1 = GetLevelData(LevelId::LEVEL_1);
std::cout << "時間限制：" << level1.timeout << "秒" << std::endl;
```

## 相關類別

- **Level**: 使用 LevelData 來初始化關卡
- **PhysicalWorld**: 關卡的物理世界實體
- **PassCondition**: 定義在 PhysicalWorld 中
