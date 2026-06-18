# Brain it On! — API 文檔

> 本文件詳細描述專案中所有公開類別、結構體、列舉與函式的 API 介面。
> 原始碼位於 `include/` 目錄，以 C++17 編寫。

---

## 目錄

- [App](App.md)
- [Constants](Constants.md)
- GameWorld 模組
  - [CoordinateHelper](GameWorld/CoordinateHelper.md)
  - [Shape（基底類別）](GameWorld/Shape/Shape.md)
  - [Rectangle](GameWorld/Shape/Rectangle.md)
  - [Circle](GameWorld/Shape/Circle.md)
  - [Capsule](GameWorld/Shape/Capsule.md)
  - [CompositeObject](GameWorld/CompositeObject/CompositeObject.md)
  - [Boundary](GameWorld/CompositeObject/Boundary.md)
  - [DrawnObject](GameWorld/CompositeObject/DrawnObject.md)
  - [MagnetObject](GameWorld/CompositeObject/MagnetObject.md)
  - [DrawingIndicator](GameWorld/DrawingIndicator.md)
  - [PhysicalWorld](GameWorld/PhysicalWorld.md)
- Level 模組
  - [LevelId](Level/LevelId.md)
  - [LevelConfig](Level/LevelConfig.md)
  - [LevelResult](Level/LevelResult.md)
  - [LevelData 全域函式](Level/LevelData.md)
  - [Level](Level/Level.md)
  - [LevelHUD](Level/LevelHUD.md)
  - [PassCondition（基底類別）](Level/PassCondition/PassCondition.md)
  - [ShapeToShapeContactPass](Level/PassCondition/ShapeToShapeContactPass.md)
  - [ShapeToAnythingContactPass](Level/PassCondition/ShapeToAnythingContactPass.md)
  - [GroupToShapeContactPass](Level/PassCondition/GroupToShapeContactPass.md)
  - [新增關卡範例](Level/Tutorial.md)
- Screen 模組（UI）
  - [ScreenType](Screen/ScreenType.md)
  - [UIScreen（基底類別）](Screen/UIScreen.md)
  - [Button](Screen/Button.md)
  - [UIElement 工廠函式](Screen/UIElement.md)
  - [LobbyScreen](Screen/LobbyScreen.md)
  - [MenuScreen](Screen/MenuScreen.md)
  - [GameScreen](Screen/GameScreen.md)
  - [ResultScreen](Screen/ResultScreen.md)
  - [SettingsScreen](Screen/SettingsScreen.md)
- Util 模組
  - [ProgressRecord](Util/ProgressRecord.md)
  - [ProgressStore](Util/ProgressStore.md)
  - [Screenshot](Util/Screenshot.md)
  - [ImageCache](Util/ImageCache.md)
