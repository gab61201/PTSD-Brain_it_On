# PTSD Brain It On! 文檔索引

本文件索引了整個專案的文檔，依照檔案位置組織。

## 目錄結構

### GameWorld - 物理世界與物件系統

| 文檔 | 說明 |
|------|------|
| [BaseObject](GameWorld/BaseObject.md) | 基礎物理物件類別 |
| [CompositeObject](GameWorld/CompositeObject.md) | 複合物件，由多個 BaseObject 組成 |
| [CoordinateHelper](GameWorld/CoordinateHelper.md) | 座標轉換工具 (像素 ↔ 公尺) |
| [DrawnObject](GameWorld/DrawnObject.md) | 玩家繪製的物件 |
| [MagnetObject](GameWorld/MagnetObject.md) | 具有磁力效果的物件 |
| [PhysicalWorld](GameWorld/PhysicalWorld.md) | 物理世界管理員 |

### Level - 關卡系統

| 文檔 | 說明 |
|------|------|
| [Level](Level/Level.md) | 關卡控制器，管理遊戲流程 |
| [LevelData](Level/LevelData.md) | 關卡配置資料結構 |
| [LevelHUD](Level/LevelHUD.md) | 畫面上方顯示的 HUD 介面 |

#### PassCondition - 過關條件檢測

| 文檔 | 說明 |
|------|------|
| [PassCondition](Level/PassCondition/PassCondition.md) | 過關條件基類 |
| [OneToOneContactPass](Level/PassCondition/OneToOneContactPass.md) | 雙物件接觸檢測條件 |

### Screen - UI 畫面系統

| 文檔 | 說明 |
|------|------|
| [UIScreen](Screen/UIScreen.md) | 抽象基類，定義所有畫面的共同介面 |
| [Button](Screen/Button.md) | 可點擊的 UI 按鈕元件 |
| [UIElement](Screen/UIElement.md) | UI 元件工具集合 |

#### 具體畫面

| 文檔 | 說明 |
|------|------|
| [MenuScreen](Screen/MenuScreen.md) | 主選單畫面 |
| [LobbyScreen](Screen/LobbyScreen.md) | 大廳畫面，供玩家選擇關卡 |
| [SettingsScreen](Screen/SettingsScreen.md) | 設定畫面 |
| [GameScreen](Screen/GameScreen.md) | 遊戲進行畫面 |

### App - 應用程式核心

| 文檔 | 說明 |
|------|------|
| [App](App.md) | 應用程式核心，管理狀態機與生命週期 |

## 系統架構概述

### 更新流程鏈

```
App::Update()
    ↓ (根據 m_ScreenType 分派)
UIScreen::Update()
    ↓
Level::Update()
    ↓ (並行執行)
├── PhysicalWorld::Update()      // 物理模擬
├── PassCondition::Update()      // 過關條件檢測
└── LevelHUD::UpdateTimer()      // HUD 更新
```

### 畫面切換流程

```
MenuScreen → LobbyScreen → GameScreen
    ↓           ↓            ↓
Settings   Level Select  Finished
```

## 相關資源

- [AGENTS.md](../AGENTS.md) - 開發者指南
- [README.md](../README.md) - 專案說明
- [CMakeLists.txt](../CMakeLists.txt) - 建置配置
- [Box2D-v2-to-v3-mapping.md](Box2D-v2-to-v3-mapping.md) - Box2D 升級映射與注意事項
