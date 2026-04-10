## Plan: Box2D v3.1.1 分階段遷移

以「完整遷移到 v3.1.1 並維持既有玩法」為目標，採分階段策略：先建立最小 Adapter 降低改動擴散，再完成核心世界/物件遷移，最後重寫接觸事件與磁鐵邏輯，並用行為回歸驗證手感。此做法可將高風險 API 斷點（Fixture 移除、ContactListener 移除、World/Body pointer 改 handle）集中處理，減少一次性爆炸修改。

**Steps**
1. Phase 0 - 基線與切面盤點（阻塞後續）
1.1 確認 CMake 目前 Box2D 來源與 link target（v2.4.1 URL、`box2d` target）作為最小可回滾基線。
1.2 以 `GameWorld`/`PassCondition` 為主建立 API 斷點清單，標註所有 `b2World`、`b2Body*`、`b2Fixture*`、`b2ContactListener` 依賴。
1.3 定義升級完成條件：可編譯、可啟動、關卡通關判定正確、磁鐵吸斥行為正確、繪線碰撞阻擋正確。

2. Phase 1 - 建置升級與最小 Adapter（阻塞 Phase 2；2.2 可平行）
2.1 升級 FetchContent Box2D 版本到 v3.1.1，檢查 v3 CMake options 與 target 名稱是否仍為 `box2d`。
2.2 建立 `PhysicsTypes`/`PhysicsApi` 薄層（只包本專案用到的少量 API），把 world/body/shape 型別從 pointer 抽象為 handle，避免業務邏輯直接綁死 C API 細節。
2.3 調整 include 策略為統一入口（優先 `box2d/box2d.h`），清理已不存在的 v2 fixture 相關 include。

3. Phase 2 - 世界與物件生命週期遷移（依賴 Phase 1）
3.1 `PhysicalWorld`：`b2World` 改 `b2WorldId`，改用 `b2WorldDef + b2CreateWorld/b2DestroyWorld`。
3.2 `CompositeObject`：`b2Body*` 改 `b2BodyId`，`CreateBody/GetPosition/GetAngle/SetType` 改為 v3 函式式呼叫。
3.3 `BaseObject`：`b2Fixture*` 改 `b2ShapeId`，`b2FixtureDef` 改 `b2ShapeDef`，circle/box 建立改用 v3 shape 建立 API。
3.4 `DrawnObject`：串接新的 body/shape API，確保繪線生成物件流程不變。
3.5 `PhysicalWorld::Update` 的 step 參數由 v2 迭代器語義切到 v3 `subStepCount`，先用官方建議值作為初始值。

4. Phase 3 - 查詢與接觸事件重構（依賴 Phase 2）
4.1 重寫 `PointQueryCallback`/`DrawingRayCastCallback` 到 v3 shape-handle callback 與 query 參數語義（ray 改 origin+translation）。
4.2 移除 `PassCondition` 對 `b2ContactListener` 的繼承，改為每幀 step 後讀取 world events（contact/sensor）並餵給條件判定。
4.3 `OneToOneContactPass` 從比較 `b2Fixture*` 改為比較對應的 `b2ShapeId`（或 owner 映射），保持既有計時器與觸發條件語義。
4.4 `MagnetObject` 由 `GetContactList` linked-list 改為事件/接觸資料查詢流程，保留「只對磁鐵生效」與力場公式。

5. Phase 4 - 行為回歸與手感調整（依賴 Phase 3）
5.1 關卡回歸：繪線可阻擋、sensor 觸發、通關計時與觸發方向（TOUCHING/SEPARATED）正確。
5.2 磁力回歸：吸引/排斥方向、最大力限制、最小距離保護維持舊版設計。
5.3 物理步進調參：調整 `subStepCount` 與必要阻尼/摩擦係數，先達成功能等價，再微調手感。

6. Phase 5 - 收斂與文件（可與 Phase 4 後段平行）
6.1 更新 README/開發者指南中的 Box2D 版本與升級注意事項。
6.2 記錄 v2→v3 映射表（舊 API 對應新 API）與已知限制，降低後續維護成本。

**Relevant files**
- `/home/brady/Dev/PTSD-Brain_it_On/CMakeLists.txt` — Box2D FetchContent 版本、CMake options、link target。
- `/home/brady/Dev/PTSD-Brain_it_On/include/GameWorld/PhysicalWorld.hpp` — world 型別與生命週期欄位。
- `/home/brady/Dev/PTSD-Brain_it_On/src/GameWorld/PhysicalWorld.cpp` — world 建立/step、AABB query、raycast。
- `/home/brady/Dev/PTSD-Brain_it_On/include/GameWorld/CompositeObject.hpp` — body 成員型別與對外介面。
- `/home/brady/Dev/PTSD-Brain_it_On/src/GameWorld/CompositeObject.cpp` — body 建立與座標同步。
- `/home/brady/Dev/PTSD-Brain_it_On/include/GameWorld/BaseObject.hpp` — fixture/shape 成員與 attach 介面。
- `/home/brady/Dev/PTSD-Brain_it_On/src/GameWorld/BaseObject.cpp` — shape 定義、sensor、材質參數。
- `/home/brady/Dev/PTSD-Brain_it_On/include/GameWorld/DrawnObject.hpp` — 繪線物件 body 介面。
- `/home/brady/Dev/PTSD-Brain_it_On/src/GameWorld/DrawnObject.cpp` — 繪線時動態建立 shape 與結束後轉 dynamic。
- `/home/brady/Dev/PTSD-Brain_it_On/src/GameWorld/MagnetObject.cpp` — 接觸迭代與磁力施加。
- `/home/brady/Dev/PTSD-Brain_it_On/include/Level/PassCondition/PassCondition.hpp` — ContactListener 舊架構入口。
- `/home/brady/Dev/PTSD-Brain_it_On/src/Level/PassCondition/PassCondition.cpp` — Begin/End 事件到條件判定橋接。
- `/home/brady/Dev/PTSD-Brain_it_On/include/Level/PassCondition/OneToOneContactPass.hpp` — one-to-one 條件狀態。
- `/home/brady/Dev/PTSD-Brain_it_On/src/Level/PassCondition/OneToOneContactPass.cpp` — fixture 比對與 timer 邏輯。
- `/home/brady/Dev/PTSD-Brain_it_On/include/GameWorld/CoordinateHelper.hpp` — `b2Vec2` 轉換工具（低風險）。

**Verification**
1. 建置驗證：`cmake -DCMAKE_BUILD_TYPE=Debug -B build && cmake --build build` 必須通過。
2. 啟動驗證：`./PTSD-Brain_it_On/PTSD-Brain_it_On` 可正常啟動且資源正常載入。
3. 互動回歸：手繪線條碰撞阻擋、放開滑鼠後動態化、關卡可達成通關。
4. 事件回歸：`TOUCHING`/`SEPARATED` 的過關條件計時語義與舊版一致。
5. 磁鐵回歸：同極相斥、異極相吸、最大力上限與最小距離保護可觀察。
6. 參數穩定性：在至少 2 個代表性關卡跑完整流程，確認未出現明顯穿透/震盪回歸。

**Decisions**
- 已確認：目標是完整遷移至 Box2D v3.1.1 並維持既有玩法行為。
- 已確認：採分階段落地（Adapter/兼容層 → 核心物理 → 接觸事件）。
- 已確認：可先做功能等價，再進行手感微調。
- 納入範圍：Build 配置、GameWorld、PassCondition、Magnet、回歸驗證與文件更新。
- 不納入範圍：新增新玩法、UI 改版、跨平台 CI 優化、非 Box2D 子系統重構。

**Further Considerations**
1. 建議先在 Phase 1 建立最小 `PhysicsApi` 封裝邊界，再進行大規模替換，能顯著降低回歸面。
2. 若要進一步壓低風險，可在 Phase 3 先把 `PassCondition` 遷移完成，再切 `MagnetObject`，避免同時改兩套接觸語義。
3. v3.1.1 為 bugfix release，但 migration 主體仍以 2.4→3.0 指南為準，需同步核對 3.1 release notes 的小 API 變更。