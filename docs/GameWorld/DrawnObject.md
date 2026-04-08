# DrawnObject

**檔案**: `include/GameWorld/DrawnObject.hpp`

玩家繪製的物件類別，繼承自 CompositeObject。

## 描述

`DrawnObject` 允許玩家在遊戲過程中動態繪製多邊形物件。玩家透過點擊螢幕來定義頂點 (Points)，系統會自動將這些頂點轉換為物理碰撞體。

## 建構式

```cpp
DrawnObject(glm::vec2 position);
```

**參數**:
- `position`: 起始位置

## 方法

### `void DrawNextPoint(glm::vec2 position)`

添加下一個頂點到繪製路徑中。

**參數**:
- `position`: 新頂點的座標

### `void EndDrawing()`

結束繪製，完成多邊形的建立並生成物理碰撞體。

### `b2Body* GetBody()`

取得此物件的 Box2D Body 指標。

**回傳值**: Box2D Body 指標 (非 const，可直接使用)

## 成員變數

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_Points` | `std::vector<glm::vec2>` | 頂點列表，儲存所有繪製的座標 |

## 繼承關係

- **繼承自**: `GameWorld::CompositeObject`
- **子類別**: 無 (最終基類)

## 使用流程

1. 建立物件：`DrawnObject obj(startPosition)`
2. 逐點繪製：多次呼叫 `DrawNextPoint(position)`
3. 完成繪製：呼叫 `EndDrawing()`
4. 取得 Body: `obj.GetBody()` 用於物理模擬

## 相關類別

- **CompositeObject**: 父類別，提供基礎複合物件功能
- **BaseObject**: BaseObject 是 CompositeObject 的組成單元
- **PhysicalWorld**: 管理所有 DrawnObject 的生命週期
