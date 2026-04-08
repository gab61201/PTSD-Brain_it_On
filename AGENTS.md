# PTSD Brain It On! - 開發者指南

## 建置與執行

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -B build  # Debug 模式為必要，否則資源無法載入
cmake --build build
./PTSD-Brain_it_On/PTSD-Brain_it_On
```

## 架構概述

- **框架**: PTSD v0.2 (Practical Tools for Simple Design) - 透過 FetchContent 獲取
- **物理引擎**: Box2D v2.4.1 - 透過 FetchContent 下載至 `lib/box2d`
- **入口點**: `src/main.cpp` 管理應用程式狀態週期（START → UPDATE → END）
- **狀態機**: App::State 列舉位於 `include/App.hpp`

### 目錄結構

| 目錄 | 內容 |
|-----------|----------|
| `src/` | 實作檔案 (.cpp) |
| `include/` | 標頭檔 (.hpp)，依子系統組織 |
| `include/GameWorld/` | 物理物件：BaseObject, CompositeObject, DrawnObject, MagnetObject, PhysicalWorld |
| `include/Level/` | 關卡系統：Level, LevelData, LevelHUD, PassCondition |
| `include/Screen/` | UI 畫面：Menu, Lobby, Game, Settings, Buttons |
| `Resources/` | 資源檔案（圖片等） |

## 框架模式

- PTSD 使用 `Core::Context::GetInstance()` 單例管理生命周期
- 應用程式狀態轉換透過 main loop 中的 switch 處理
- 物理物件對應至 Box2D Body（參見 `GameWorld/PhysicalWorld.hpp`）

### Update() 資料傳遞鍊

#### App → Screen

`App::Update()` 根據 `m_ScreenType` 分派至對應的 `UIScreen`：

```
App::Update()
  → switch (m_ScreenType) [include/App.cpp:47]
    → m_Screen->Update() [include/App.hpp:33]
```

#### Screen → Level

`GameScreen::Update()` 將更新責任轉發給關卡：

```
UI::GameScreen::Update() [include/Screen/GameScreen.hpp:13]
  → m_Level.Update() [include/Level/Level.hpp:19]
```

#### Level → Subsystems

`Level::Update()` 負責協調多個子系統，每幀依序呼叫：

```
Level::Update() [include/Level/Level.hpp:19]
  → m_World->Update() [include/GameWorld/PhysicalWorld.hpp:22]     // 物理世界更新
  → PassCondition::Update() [include/Level/PassCondition/PassCondition.hpp:30]  // 過關條件檢查
  → m_HUD->UpdateTimer() [include/Level/LevelHUD.hpp:17]           // 計時器顯示更新
```

**執行順序**: 每幀由上而下逐層呼叫，各層負責各自子系統更新。