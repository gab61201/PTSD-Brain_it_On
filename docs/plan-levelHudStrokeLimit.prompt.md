## Plan: Add stroke limit HUD

在 `LevelData` 中新增每關的 `strokeLimit`，預設先統一為 `3`，再讓 `LevelHUD` 在筆劃圖示下方顯示 `remaining/total`。目前不做筆劃上限封鎖，只顯示數值，且剩餘筆劃允許繼續往負數累加，符合你的需求。

**Steps**

1. 先擴充關卡資料來源，讓筆劃限制成為每關設定值。更新 `include/Level/LevelData.hpp` 的 `LevelData` 結構與 `docs/Level/LevelData.md`，並在 `src/Level/LevelData/Level1.cpp`, `src/Level/LevelData/Level2.cpp`, `src/Level/LevelData/Level3.cpp` 內都填入同一個預設值 `3`。這一步是後續 HUD 計算的資料基礎。
2. 再擴充 HUD 介面，讓 `LevelHUD` 持有一個新的文字物件來顯示筆劃次數。修改 `include/Level/LevelHUD.hpp` 與 `src/Level/LevelHUD.cpp`，在既有的 stroke limit 圖示下方建立文字，格式固定為 `remaining/total`，例如 `3/3`、`0/3`、`-1/3`。
3. 把實際剩餘筆劃的計算串到 `Level` 更新流程。修改 `include/Level/Level.hpp` 與 `src/Level/Level.cpp`，在初始化與重設時把 `strokeLimit` 傳進 HUD，並在每幀更新時用目前已建立的筆劃數推算剩餘值，再呼叫 HUD 更新文字。這一步只負責顯示，不改變繪圖流程。
4. 補齊物件數量的查詢能力。若目前 `PhysicalWorld` 沒有公開已畫筆劃數，則在 `include/GameWorld/PhysicalWorld.hpp` 與對應實作中新增唯讀 accessor，讓 `Level` 能取得目前 draw object 數量，而不用直接碰私有容器。
5. 同步文件與行為說明。更新 `docs/Level/LevelHUD.md`, `docs/Level/Level.md`, `docs/Level/LevelData.md`，把新 HUD 元素、資料欄位與「剩餘筆劃可為負數、暫不封鎖繪圖」的行為寫清楚。
6. 完成後做一次完整驗證，確認 HUD 顯示位置、數字格式與重置行為都正確，再依你要求整理成單一 Conventional Commit。

**Relevant files**

- `/home/brady/Dev/PTSD-Brain_it_On/include/Level/LevelData.hpp` — 新增 `strokeLimit` 欄位。
- `/home/brady/Dev/PTSD-Brain_it_On/src/Level/LevelData/Level1.cpp` — 設定關卡 1 的筆劃限制。
- `/home/brady/Dev/PTSD-Brain_it_On/src/Level/LevelData/Level2.cpp` — 設定關卡 2 的筆劃限制。
- `/home/brady/Dev/PTSD-Brain_it_On/src/Level/LevelData/Level3.cpp` — 設定關卡 3 的筆劃限制。
- `/home/brady/Dev/PTSD-Brain_it_On/include/Level/LevelHUD.hpp` — 新增 stroke 文字狀態與更新介面。
- `/home/brady/Dev/PTSD-Brain_it_On/src/Level/LevelHUD.cpp` — 建立與更新筆劃顯示文字。
- `/home/brady/Dev/PTSD-Brain_it_On/include/Level/Level.hpp` — 讓 Level 持有或傳遞 strokeLimit 相關資料。
- `/home/brady/Dev/PTSD-Brain_it_On/src/Level/Level.cpp` — 每幀計算剩餘筆劃並更新 HUD。
- `/home/brady/Dev/PTSD-Brain_it_On/include/GameWorld/PhysicalWorld.hpp` — 必要時新增 draw count 讀取方法。
- `/home/brady/Dev/PTSD-Brain_it_On/docs/Level/LevelHUD.md` — 文件補充新的 HUD 元素與格式。
- `/home/brady/Dev/PTSD-Brain_it_On/docs/Level/LevelData.md` — 文件補充 `strokeLimit` 欄位。
- `/home/brady/Dev/PTSD-Brain_it_On/docs/Level/Level.md` — 文件補充關卡更新與限制筆劃的互動說明。

**Verification**

1. 重新建置專案，確認新增欄位與 HUD 文字後沒有編譯錯誤。
2. 進入關卡後觀察 stroke limit 圖示下方是否顯示 `3/3`，開始畫一次後是否變成 `2/3`，繼續超過限制後是否能看到負數。
3. 重新開始關卡後確認筆劃文字會回到初始值，且 target text 與 timer 的既有行為不受影響。
4. 如果有現成測試或可手動進入的場景，優先用實際遊玩驗證，因為這個改動主要是 UI 與遊戲流程整合。

**Decisions**

- 先做顯示，不做筆劃封鎖。
- 筆劃限制採每關配置，先全部統一為 `3`。
- 剩餘筆劃允許小於 0，格式仍維持 `remaining/total`。
- 文件會一起更新，避免 UI 與說明脫節。
