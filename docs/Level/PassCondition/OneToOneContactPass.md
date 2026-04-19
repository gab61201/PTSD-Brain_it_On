# OneToOneContactPass

**檔案**: `include/Level/PassCondition/OneToOneContactPass.hpp`

特定的過關條件：當兩個指定物件接觸 (或分離) 時達成。

## 描述

`OneToOneContactPass` 是 `PassCondition` 的具體實作，用於檢測兩個特定 Shape 之間的互動狀態。當它們達到指定的觸發條件並持續足夠時間後，即視為過關。也支援單一物件自我檢測 (如檢測同一物件的多個 fixture 接觸)。

## 建構式

### 版本一：雙物件接觸

```cpp
OneToOneContactPass(
    b2ShapeId shapeA,           // Shape A handle
    b2ShapeId shapeB,           // Shape B handle
    TriggerType triggerType,    // 觸發類型：接觸或分離
    int duration                // 需要持續的幀數
);
```

### 版本二：單一物件自我檢測

```cpp
OneToOneContactPass(
    b2ShapeId shape,            // Shape handle
    TriggerType triggerType,    // 觸發類型
    int duration                // 需要持續的幀數
);
```

**參數說明**:
- `shapeA` / `shapeB`: 要檢測的兩個 shape handle (`b2ShapeId`)
- `triggerType`: 接觸 (TOUCHING) 或分離 (SEPARATED)
- `duration`: 條件需維持的幀數，防止誤判

## 方法

### `void OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) override`

處理碰撞事件的內部實作。當指定的兩個 shape 發生接觸或分離時呼叫。會檢查事件中的 shape handle 是否與預設的 shapeA/shapeB 匹配，若匹配則遞增計時器；否則重設計時器。

**參數**:
- `shapeA`: 第一個碰撞 shape handle
- `shapeB`: 第二個碰撞 shape handle
- `triggerType`: 觸發類型

## 成員變數 (私有)

| 名稱 | 類型 | 說明 |
|------|------|------|
| `m_ShapeA` | `b2ShapeId` | Shape A handle (`b2_nullShapeId` 表示未設定) |
| `m_ShapeB` | `b2ShapeId` | Shape B handle (`b2_nullShapeId` 表示未設定) |

## 繼承關係

- **繼承自**: `PassCondition`
- **子類別**: 無

## 使用流程

```cpp
// 建立過關條件：當 shapeA 與 shapeB 接觸持續 60 幀即過關
auto condition = std::make_shared<OneToOneContactPass>(shapeA, shapeB, TriggerType::TOUCHING, 60);

// 每幀檢查
if (condition->Check(physicalWorld->GetContactEvents())) {
    // 過關！
}
```

## 相關類別

- **PassCondition**: 父類別，過關條件基類
- **TriggerType**: 觸發類型列舉
- **PhysicalWorld**: 提供 contact events
