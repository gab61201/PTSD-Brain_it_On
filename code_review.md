# PTSD-Brain_it_On — Code Review

> 審查範圍：專案全部 `.cpp` / `.hpp` 檔案、`CMakeLists.txt` 及 `files.cmake`
> 審查時間：2026-05-22

---

## 🔴 嚴重問題（Critical Bugs）

### 1. `Level::Update()` 中超時後遊戲不會結束

[Level.cpp:73-100](file:///home/gab/Code/PTSD-Brain_it_On/src/Level/Level.cpp#L73-L100)

當 `GetRemainingTime()` 歸零後，`m_state` 永遠不會變成 `FINISHED`——玩家可以繼續無限操作。需要在 `Update()` 中加上超時檢查：

```diff
 void Level::Update() {
     if (m_state == State::DRAWING || m_state == State::PLAYING) {
         m_Time += static_cast<float>(Util::Time::GetDeltaTimeMs()) / 1000.0f;
     }
+
+    // 超時判斷
+    if (m_state != State::FINISHED && m_state != State::WAITING && GetRemainingTime() <= 0.0f) {
+        m_state = State::FINISHED;
+        Save();
+        m_World->Stop();
+        return;
+    }
```

### 2. `Level::Playing()` 中 `m_PassCondition` null check 在解引用之後

[Level.cpp:52-55](file:///home/gab/Code/PTSD-Brain_it_On/src/Level/Level.cpp#L52-L55)

```cpp
int contactCountDown = m_PassCondition->GetContactCountDown(); // ← 可能為 null
// ...
if (m_PassCondition && m_PassCondition->Check(...)) {           // ← 太晚檢查
```

第 52 行已經解引用了 `m_PassCondition`，第 55 行才做 null check，**順序顛倒**。應將 null check 移到最前面，或者在建構時保證 `m_PassCondition` 永不為 null。

### 3. `Level::Update()` 中 `m_World` 可能為 null 卻仍被解引用

[Level.cpp:86](file:///home/gab/Code/PTSD-Brain_it_On/src/Level/Level.cpp#L86)

```cpp
m_HUD->UpdateStrokeLimit(m_StrokeLimit - m_World->GetDrawnObjectCount(), m_StrokeLimit);
```

第 80 行有檢查 `m_World` 是否為 null，但第 86 行沒有保護就直接存取 `m_World->GetDrawnObjectCount()`。

### 4. `App::Update()` 中 `static_cast` 向下轉型不安全

[App.cpp:35](file:///home/gab/Code/PTSD-Brain_it_On/src/App.cpp#L35)

```cpp
m_SelectedLevelId = static_cast<UI::MenuScreen*>(m_Screen.get())->GetSelectedLevelId();
```

這裡用 `static_cast` 向下轉型，沒有任何型別安全保障。如果切換邏輯有 bug（例如 `m_Screen` 實際不是 `MenuScreen`），會造成 **undefined behavior**。建議改用 `dynamic_cast` 並檢查結果，或重新設計讓 `m_SelectedLevelId` 透過其他機制傳遞。

### 5. `ResultScreen` 接收的 `LevelId*` 指標被忽略

[ResultScreen.cpp:25-27](file:///home/gab/Code/PTSD-Brain_it_On/src/Screen/ResultScreen.cpp#L25-L27)

```cpp
ResultScreen::ResultScreen(LevelId* levelId)
    : m_ResultData(Util::ProgressStore::s_LastPlayedLevelConfig) {
    (void)levelId;  // ← 刻意忽略？
```

建構子接受指標參數卻用 `(void)` 吞掉。如果這個參數是為了「下一關」功能準備的，那傳裸指標是個危險的設計；如果已經不需要了，應該把它從介面移除。

---

## 🟠 架構與設計（Architecture & Design）

### 6. `ProgressStore::s_LastPlayedLevelConfig` 破壞封裝

[ProgressStore.hpp:34](file:///home/gab/Code/PTSD-Brain_it_On/include/Util/ProgressStore.hpp#L34)

```cpp
static LevelResult s_LastPlayedLevelConfig;  // public static
```

`ResultScreen` 透過這個全域可變靜態成員與 `Level::Save()` 溝通。這是隱式的資料流，破壞模組間的封裝性：

- 呼叫端必須**先**呼叫 `Save()` 才能**之後**讀到正確值，但編譯器不會強制這個順序
- 建議改為：`ResultScreen` 建構時直接接收 `LevelResult` 值，由 `GameScreen` 在過關時傳遞

### 7. 靜態自動註冊 pattern 的初始化順序風險

[Level1.cpp:67-74](file:///home/gab/Code/PTSD-Brain_it_On/src/Level/LevelConfig/Level1.cpp#L67-L74)

```cpp
namespace {
struct Register {
    Register() { RegisterLevel(LevelId::LEVEL_1, LevelConfig_1); }
};
static Register reg;
}
```

雖然用了 `GetLevelRegistry()` 的 function-local static 避免 static initialization order fiasco，但 `Register::Register()` 的呼叫時機仍然取決於各翻譯單元的載入順序。目前可行是因為 `GetLevelRegistry()` 正確使用了 local static，但若未來有其他 static 物件依賴 registry 的內容就會出問題。建議在文件或程式碼中加入明確的生命週期說明。

### 8. `LevelId` enum 與 `MENU_CARD_COUNT` 不同步

[LevelData.hpp:10-16](file:///home/gab/Code/PTSD-Brain_it_On/include/Level/LevelData.hpp#L10-L16) vs [MenuScreen.cpp:15](file:///home/gab/Code/PTSD-Brain_it_On/src/Screen/MenuScreen.cpp#L15)

`LevelId` 目前只定義了 5 個關卡（`LEVEL_1` ~ `LEVEL_5`），但 `MenuScreen` 顯示 10 張卡片 (`MENU_CARD_COUNT = 10`)。點擊未註冊的關卡會在 `GetLevelConfig()` 中拋出 `std::runtime_error`，導致**直接 crash**。

建議：
- 讓 `MENU_CARD_COUNT` 等於實際已註冊的關卡數
- 或在 `MenuScreen` 中檢查 registry，對未註冊的關卡顯示為「鎖定」狀態

### 9. `SquareButton` 與 `CircleButton` 完全相同

[UIElement.cpp:18-32](file:///home/gab/Code/PTSD-Brain_it_On/src/Screen/UIElement.cpp#L18-L32)

兩個函式的實作一模一樣，只是名稱不同。如果不打算讓 `CircleButton` 做圓形 hit test，那應該合併為一個函式；若將來要區分，請加上 TODO 註解說明預期行為。

### 10. `Button::OnPressing()` 是空殼方法

[Button.cpp:32-36](file:///home/gab/Code/PTSD-Brain_it_On/src/Screen/Button.cpp#L32-L36)

```cpp
void Button::OnPressing() {
    if (IsCursorPointing() || Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        // scale to 1.1
    }
}
```

函式體為空，且從未被呼叫。這是待完成的功能還是被遺忘的死碼？應該實作或刪除。

### 11. `SettingsScreen` 沒有初始化 `m_NextScreenType`

[SettingsScreen.cpp:9-15](file:///home/gab/Code/PTSD-Brain_it_On/src/Screen/SettingsScreen.cpp#L9-L15)

其他 Screen 都會在建構子中設定 `m_NextScreenType = ScreenType::XXX`，但 `SettingsScreen` 沒有。基類 `UIScreen` 中 `m_NextScreenType` **沒有預設值**，所以這是**未初始化的成員**，讀取它是 undefined behavior。

---

## 🟡 安全與穩健性（Safety & Robustness）

### 12. `PassCondition::Check()` 中的 off-by-one 與 timing 問題

[PassCondition.cpp:13-29](file:///home/gab/Code/PTSD-Brain_it_On/src/Level/PassCondition/PassCondition.cpp#L13-L29)

```cpp
bool PassCondition::Check(b2ContactEvents events) {
    if (m_Timer > m_Duration * FPS) {   // (A) 先檢查
        return true;
    } else if (m_Timer > 0) {
        m_Timer++;                       // (B) 再遞增
    }
    // ... OnContactEvent 可能設定 m_Timer = 1
    return m_Timer > m_Duration * FPS;   // (C) 再檢查一次
}
```

問題：
- **(A)** 和 **(C)** 是 `>` 而不是 `>=`，所以實際要等 `m_Duration * FPS + 1` 幀才過關，多了一幀
- `m_Timer` 混合了「幀數計數器」與「是否啟動」兩種語意，閱讀困難
- 如果 `m_Duration == 0`，則 `m_Timer = 1 > 0 * 60 = 0`，在**下一幀**的 **(A)** 才會返回 true——預期應該是立即通過

### 13. `GetContactCountDown()` 計算可能回傳負數

[PassCondition.cpp:32-37](file:///home/gab/Code/PTSD-Brain_it_On/src/Level/PassCondition/PassCondition.cpp#L32-L37)

```cpp
int PassCondition::GetContactCountDown() const {
    if (m_Timer > 0) {
        return m_Duration - m_Timer / FPS;
    }
    return 0;
}
```

當 `m_Timer > m_Duration * FPS` 時，`m_Duration - m_Timer / FPS` 會回傳**負數**。雖然 HUD 中用 `> 0` 過濾，但函式本身的合約不清楚。建議加上 `std::max(0, ...)` 或在文件中說明。

### 14. `DrawnObject::m_Points` 是 public 成員

[DrawnObject.hpp:19](file:///home/gab/Code/PTSD-Brain_it_On/include/GameWorld/CompositeObject/DrawnObject.hpp#L19)

```cpp
std::vector<glm::vec2> m_Points;  // public!
```

物理世界的核心資料直接暴露在外，任何人都能修改。且 `PhysicalWorld` 中直接存取 `m_LastDrawingObject->m_Points.back()` ([PhysicalWorld.cpp:114](file:///home/gab/Code/PTSD-Brain_it_On/src/GameWorld/PhysicalWorld.cpp#L114))，應該提供 getter。

### 15. `DrawnObject` 中 `Getb2BodyId()` 隱藏了基類方法

[DrawnObject.hpp:17](file:///home/gab/Code/PTSD-Brain_it_On/include/GameWorld/CompositeObject/DrawnObject.hpp#L17) vs [CompositeObject.hpp:37](file:///home/gab/Code/PTSD-Brain_it_On/include/GameWorld/CompositeObject/CompositeObject.hpp#L37)

`DrawnObject` 定義了非 virtual 的 `Getb2BodyId()`，基類 `CompositeObject` 也有同名方法。這是 **name hiding**，不是 override，容易造成混淆。建議直接使用基類的版本。

### 16. `Level(LevelId)` 建構子應標記為 `explicit`

[Level.hpp:11](file:///home/gab/Code/PTSD-Brain_it_On/include/Level/Level.hpp#L11)

單參數建構子未標記 `explicit`，允許隱式轉換 `LevelId → Level`，這幾乎不會是你想要的行為。

### 17. `LevelId` 的 `std::unordered_map` 缺少 hash 特化

[LevelData.hpp:40](file:///home/gab/Code/PTSD-Brain_it_On/include/Level/LevelData.hpp#L40)

```cpp
std::unordered_map<LevelId, LevelFunction>& GetLevelRegistry();
```

`LevelId` 是 `enum class`，在 C++17 中 `std::hash<LevelId>` 已有特化所以能編譯，但依賴此行為不太明確。如果有同事使用較舊的編譯器或 stdlib 版本可能會壞掉。加一行 hash 特化或改用 `std::map` 更安全。

---

## 🔵 命名與風格（Naming & Style）

### 18. 檔案命名 vs 類別命名不一致

| 檔案 | 內容 |
|------|------|
| `LevelData.hpp` | 定義了 `LevelConfig`、`LevelResult`、`LevelId`、`LevelFunction` |

檔案名叫 `LevelData` 但已經沒有任何叫 `LevelData` 的型別了（你剛把它重命名為 `LevelConfig`）。檔名應更新為 `LevelConfig.hpp` 或更通用的 `LevelTypes.hpp`。

### 19. Include guard 命名不一致

| 檔案 | Guard |
|------|-------|
| `Level.hpp` | `LEVEL_HPP` |
| `PhysicalWorld.hpp` | `PHYSICAL_WORLD_HPP` |
| `CoordinateHelper.hpp` | `GAMEWORLD_COORDINATE_HELPER_HPP` |

部分有加命名空間前綴（`GAMEWORLD_`），部分沒有。建議統一，例如都使用 `NAMESPACE_FILENAME_HPP` 格式。或者直接用 `#pragma once`。

### 20. 變數命名不一致

- [UIElement.cpp:19-20](file:///home/gab/Code/PTSD-Brain_it_On/src/Screen/UIElement.cpp#L19-L20)：區域變數 `ButtonImage`、`Button` 使用 PascalCase，但專案其他地方都用 camelCase
- [LevelHUD.cpp:12](file:///home/gab/Code/PTSD-Brain_it_On/src/Level/LevelHUD.cpp#L12)：`LEFT_UI_X` 是 `constexpr float` 但用全大寫（通常只用於 macro）

### 21. `FINISHED` 狀態在 `switch` 中無 case

[Level.cpp:89-100](file:///home/gab/Code/PTSD-Brain_it_On/src/Level/Level.cpp#L89-L100)

```cpp
switch (m_state) {
    case State::WAITING: ...
    case State::DRAWING: ...
    case State::PLAYING: ...
    // ← FINISHED 沒有 case，也沒有 default
}
```

雖然 `FINISHED` 時不需要做事，但建議加上 `case State::FINISHED: break;` 或 `default: break;` 以消除編譯器 `-Wswitch` 警告並表明有意為之。

---

## ⚡ 效能（Performance）

### 22. `LevelConfig` 回傳值觸發 deep copy

[LevelData.cpp:22](file:///home/gab/Code/PTSD-Brain_it_On/src/Level/LevelData.cpp#L22)

```cpp
return it->second();  // 呼叫 LevelFunction，回傳 LevelConfig
```

`LevelConfig` 包含兩個 `shared_ptr`，每次呼叫都會**重新建立整個物理世界**（包括所有 Box2D bodies）。這是 `Reset()` 刻意設計的（每次重建世界），但要注意這不是輕量操作。

### 23. `GameScreen::Update()` 中 for loop 用值拷貝

[GameScreen.cpp:40](file:///home/gab/Code/PTSD-Brain_it_On/src/Screen/GameScreen.cpp#L40)

```cpp
for (auto button : m_Buttons) {  // ← 拷貝 shared_ptr
```

應該改為 `const auto&` 以避免不必要的 `shared_ptr` 引用計數增減（`LobbyScreen` 也一樣）。`ResultScreen` 和 `MenuScreen` 已正確使用 `const auto&`。

### 24. `Constants.hpp` 中 `#include <string>` 放在常數定義之後

[Constants.hpp:15](file:///home/gab/Code/PTSD-Brain_it_On/include/Constants.hpp#L15)

Include 應該放在檔案最前面（header guard 之後）。目前的位置雖然功能上正確，但不符合慣例，讀者可能會忽略它。

---

## 🛠️ 可維護性（Maintainability）

### 25. Boundary 依靠 magic index 存取牆壁

[Level2.cpp:28](file:///home/gab/Code/PTSD-Brain_it_On/src/Level/LevelConfig/Level2.cpp#L28)、[Level3.cpp:42](file:///home/gab/Code/PTSD-Brain_it_On/src/Level/LevelConfig/Level3.cpp#L42)

```cpp
boundary->GetShapes()[0]->Getb2ShapeId()   // Level2：左牆？
boundary->GetShapes()[3]->Getb2ShapeId()   // Level3：底牆？
```

用 magic number 索引依賴 `Boundary` 建構子中 Shape 的建立順序，極度脆弱。一旦 `Boundary` 改變內部 Shape 的順序或數量，所有關卡都會悄悄壞掉。

建議在 `Boundary` 加上具名 getter：
```cpp
b2ShapeId GetLeftWallShapeId() const;
b2ShapeId GetRightWallShapeId() const;
b2ShapeId GetTopWallShapeId() const;
b2ShapeId GetBottomWallShapeId() const;
```

### 26. `Level::Save()` 在返回選單時也被呼叫

[GameScreen.cpp:15-18](file:///home/gab/Code/PTSD-Brain_it_On/src/Screen/GameScreen.cpp#L15-L18)

```cpp
auto backButton = UI::Element::CircleButton([this] {
    m_Level.Save();           // ← 未完成時也存檔
    m_NextScreenType = ScreenType::MENU;
}, Path::BtnBack);
```

`Save()` 是 public 的，返回選單時存檔意味著**未通關的記錄**也會被寫入。雖然 `ApplyResultAndSave` 中有 `isNewRecord` 保護，但語意上仍然奇怪——「返回選單」不應該觸發「存檔」行為。

### 27. `PhysicalWorld` 建構子的 boundary 預設值放在 `.cpp`

[PhysicalWorld.cpp:58](file:///home/gab/Code/PTSD-Brain_it_On/src/GameWorld/PhysicalWorld.cpp#L58)

```cpp
PhysicalWorld::PhysicalWorld(
    std::vector<std::shared_ptr<CompositeObject>> compositeObjects,
    std::shared_ptr<Boundary> boundary = nullptr)  // ← 預設值在定義處
```

預設參數值應該放在**宣告**（`.hpp`）中，而非定義（`.cpp`）中。目前寫法在某些情況下可能導致編譯器找不到預設值。

---

## 🔧 CMake 配置

### 28. `CMakeLists.txt` 中 Debug/Release 邏輯重複

[CMakeLists.txt:53-86](file:///home/gab/Code/PTSD-Brain_it_On/CMakeLists.txt#L53-L86)

`if(CMAKE_BUILD_TYPE STREQUAL "Debug")` 區塊出現了兩次，且兩次對 Debug 的定義完全相同。第二個 `else` 分支新增了 PTSD 覆寫和 assets 複製。這兩段應該合併為一個 if/else。

```diff
 if(CMAKE_BUILD_TYPE STREQUAL "Debug")
     target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
 else()
     target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="Resources")
     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory
         ${CMAKE_CURRENT_SOURCE_DIR}/Resources
         $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources)
+    target_compile_definitions(PTSD PRIVATE PTSD_ASSETS_DIR="assets/")
+    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
+        COMMAND ${CMAKE_COMMAND} -E copy_directory
+        ${ptsd_SOURCE_DIR}/assets
+        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
 endif()
-
-if(CMAKE_BUILD_TYPE STREQUAL "Debug")
-    ...（重複的區塊刪除）
-endif()
```

---

## 📊 總結

| 嚴重度 | 數量 | 代表問題 |
|--------|------|----------|
| 🔴 Critical | 5 | 超時不結束、null 解引用順序錯誤、unsafe downcast |
| 🟠 Design | 6 | 全域靜態耦合、enum/card count 不同步、dead code |
| 🟡 Safety | 6 | off-by-one timer、public 成員、name hiding |
| 🔵 Style | 4 | 檔名 vs 類名不一致、switch 缺 default |
| ⚡ Perf | 3 | shared_ptr for loop 拷貝 |
| 🛠️ Maint | 3 | magic index、不當存檔觸發 |
| 🔧 CMake | 1 | 重複的 Debug/Release 區塊 |

> [!IMPORTANT]
> 最建議優先修復的是 **#1 超時不結束**、**#2 null check 順序**、**#3 m_World null 解引用** 以及 **#8 LevelId 與 MENU_CARD_COUNT 不同步**，這四個問題會直接導致運行時崩潰或邏輯錯誤。
