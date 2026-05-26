# AGENTS.md — Brain it On! (PTSD-Brain_it_On)

## 專案概述

Brain it On! 是一款以 **Box2D v3** 為物理引擎的 2D 物理益智遊戲，基於 **PTSD（Practical Tools for Simple Design）** 框架開發。玩家在畫面上繪製線條、形狀，利用重力與碰撞原理達成各關卡的特定目標（如讓兩物體接觸）。遊戲以最少筆畫、最短時間為評分標準，超時則失敗。

- **語言 / 標準**：C++17
- **建置系統**：CMake ≥ 3.16
- **外部依賴**（透過 FetchContent 自動拉取）：
  - [PTSD v0.2](https://github.com/ntut-open-source-club/practical-tools-for-simple-design) — 渲染、輸入、音訊、資源管理框架
  - [Box2D v3.1.1](https://github.com/erincatto/box2d) — 物理模擬引擎
  - [nlohmann/json v3.12.0](https://github.com/nlohmann/json) — JSON 解析（備用 / 未來資料序列化）
  - SDL2 — 底層視窗與輸入（由 PTSD 間接引入）

---

## 目錄結構

```
PTSD-Brain_it_On/
├── CMakeLists.txt          # 頂層建置腳本
├── files.cmake             # GLOB 收集 src/*.cpp 與 include/*.hpp
├── include/                # 所有標頭檔（公開 API）
│   ├── App.hpp
│   ├── Constants.hpp
│   ├── GameWorld/           # 物理世界 + 形狀 + 複合物件
│   │   ├── CoordinateHelper.hpp
│   │   ├── DrawingIndicator.hpp
│   │   ├── PhysicalWorld.hpp
│   │   ├── Shape/           # Shape, Rectangle, Circle, Capsule
│   │   └── CompositeObject/ # CompositeObject, Boundary, DrawnObject, MagnetObject
│   ├── Level/               # 關卡邏輯 + 過關條件
│   │   ├── Level.hpp
│   │   ├── LevelData.hpp
│   │   ├── LevelHUD.hpp
│   │   └── PassCondition/   # PassCondition, OneToOneContactPass
│   ├── Screen/              # UI 畫面系統
│   │   ├── UIScreen.hpp
│   │   ├── UIElement.hpp
│   │   ├── Button.hpp
│   │   ├── LobbyScreen.hpp
│   │   ├── MenuScreen.hpp
│   │   ├── GameScreen.hpp
│   │   ├── ResultScreen.hpp
│   │   └── SettingsScreen.hpp
│   └── Util/                # 工具類
│       ├── ProgressStore.hpp
│       └── Screenshot.hpp
├── src/                    # 所有實作檔
│   ├── main.cpp
│   ├── App.cpp
│   ├── GameWorld/
│   ├── Level/
│   │   └── LevelConfig/    # Level1.cpp ~ Level5.cpp（關卡定義）
│   ├── Screen/
│   └── Util/
└── Resources/              # 執行期資源
    ├── Audios/
    ├── Fonts/
    ├── Images/
    └── Save/               # 玩家存檔（CSV）
```

---

## 架構與核心概念

### 遊戲迴圈

`main.cpp` → `Core::Context` → `App`（狀態機：`START` → `UPDATE` → `END`）

`App::Update()` 每幀呼叫當前 `UIScreen::Update()`，根據回傳的 `ScreenType` 決定畫面切換。

### 畫面流程（Screen Flow）

```
LobbyScreen → MenuScreen → GameScreen → ResultScreen
                  ↑              ↓            ↓
                  └──────────────┴────────────┘
```

所有畫面繼承 `UI::UIScreen`，實作 `Update()` 與 `GetScreenType()`。

### 物理世界（GameWorld）

| 類別 | 責任 |
|------|------|
| `PhysicalWorld` | 持有 `b2WorldId`，管理所有 `CompositeObject` 與 `DrawnObject` |
| `CompositeObject` | 多個 `Shape` 組成的剛體（靜態 / 動態 / 運動學） |
| `DrawnObject` | 玩家繪製產生的動態物件，繼承 `CompositeObject` |
| `MagnetObject` | 帶磁力的特殊物件，繼承 `CompositeObject` |
| `Boundary` | 關卡邊界牆壁，繼承 `CompositeObject` |
| `Shape` (abstract) | 基底形狀，子類：`Rectangle`、`Circle`、`Capsule` |
| `CoordinateHelper` | 像素 ↔ Box2D 公尺座標轉換（50 px/m） |
| `DrawingIndicator` | 繪製時的即時視覺回饋線段 |

### 關卡系統（Level）

- `LevelData.hpp` 定義 `LevelId`（enum）、`LevelConfig`、`LevelResult`。
- 每個關卡透過 **靜態自動註冊** 模式（`RegisterLevel()`）在 `src/Level/LevelConfig/LevelN.cpp` 中定義。
- `Level` 類別管理關卡狀態機：`WAITING → DRAWING → PLAYING → FINISHED`。
- `PassCondition`（抽象）及 `OneToOneContactPass` 定義過關碰撞條件。

### 進度存檔（ProgressStore）

- 使用 CSV 格式（`Resources/Save/BestRecord.csv`）儲存每關最佳紀錄。
- `ProgressStore` 為全靜態類別，提供 `LoadOrCreateDefault()`、`ApplyResultAndSave()`、`GetStars()` 等方法。
- 三星條件：通關、時間限制內、筆畫限制內。

---

## 開發慣例

### 命名規則

| 項目 | 風格 | 範例 |
|------|------|------|
| 類別 / 結構體 | PascalCase | `PhysicalWorld`, `LevelConfig` |
| 成員變數 | `m_` 前綴 + PascalCase | `m_CurrentState`, `m_b2WorldId` |
| 靜態成員 | `s_` 前綴 + PascalCase | `s_ImageCache`, `s_Records` |
| 全域常數 | `k` 前綴 + PascalCase 或 UPPER_SNAKE | `kPixelsPerMeter`, `RESOLUTION_X` |
| 函式 / 方法 | PascalCase | `GetStars()`, `AttachToBody()` |
| 命名空間 | PascalCase | `GameWorld`, `UI::Element` |
| 列舉值 | UPPER_SNAKE_CASE | `LEVEL_1`, `STATIC`, `TOUCHING` |
| 檔案 | PascalCase | `PhysicalWorld.hpp`, `LevelData.cpp` |

### 程式碼風格

- 使用 `.clang-format` 自動格式化。
- 標頭檔使用 `#pragma once`。
- 盡可能使用 `std::shared_ptr` / `std::unique_ptr` 進行記憶體管理。
- Box2D ID 類型（`b2WorldId`, `b2BodyId`, `b2ShapeId`）使用 Box2D v3 的 handle-based API。
- 座標系統：畫面中心為原點，右為正 X，上為正 Y。

### 新增關卡流程

1. 在 `src/Level/LevelConfig/` 新增 `LevelN.cpp`。
2. 在 `LevelId` 列舉加入新 ID。
3. 實作 `LevelConfig LevelConfig_N()` 函式，配置物件、邊界、過關條件。
4. 使用靜態註冊模式（匿名 namespace + static 物件）將關卡自動註冊。
5. 無需修改 `files.cmake`（GLOB_RECURSE 自動收集）。

### 建置指令

```bash
# 設定建置（必須使用 Debug 模式）
cmake -B build -DCMAKE_BUILD_TYPE=Debug # -G Ninga

# 編譯
cmake --build build -j

# 執行
./build/PTSD-Brain_it_On
```

### 資源路徑

- Debug 模式：`RESOURCE_DIR` 指向原始碼中的 `Resources/` 目錄。
- 程式碼中使用 `RESOURCE_DIR` 巨集或 `Constants.hpp` 中定義的 `Path::` 常數取得資源路徑。

---

## 注意事項

- 本專案使用 **Box2D v3**（非 v2），API 風格為 C-like handle（`b2WorldId`、`b2BodyId`），而非物件導向的 `b2World*`、`b2Body*`。
- PTSD 框架提供 `Util::GameObject`、`Util::Renderer`、`Util::Image`、`Util::Input` 等封裝，不要直接使用 SDL2 API。
- `PhysicalWorld` 的建構子會自動呼叫各 `CompositeObject::AttachToWorld()` 將物件掛載到 Box2D 世界。
- 所有 UI 畫面的事件處理邏輯都在 `Update()` 方法中完成，透過回傳不同的 `ScreenType` 通知 `App` 進行畫面切換。
- **修改完程式碼後，請務必檢查並同步修改 `docs/` 目錄下對應的 API 文件，保持文件與程式碼的一致性。**
