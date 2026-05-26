# PassCondition

**標頭檔**：[`include/Level/PassCondition/PassCondition.hpp`](../../../include/Level/PassCondition/PassCondition.hpp)

過關條件的抽象基底類別。

#### `TriggerType` 列舉

```cpp
enum class TriggerType {
    TOUCHING,   // 兩物體接觸時觸發
    SEPARATED   // 兩物體分離時觸發
};
```

#### 建構子

```cpp
PassCondition(TriggerType triggerType, int duration);
```

#### 方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `Check` | `bool Check(b2ContactEvents events)` | 檢查碰撞事件是否滿足過關條件 |
| `GetContactCountDown` | `int GetContactCountDown() const` | 取得接觸倒數計時器剩餘值 |

#### 純虛方法（Protected）

```cpp
virtual void OnContactEvent(
    b2ShapeId shapeA,
    b2ShapeId shapeB,
    TriggerType triggerType
) = 0;
```

#### Protected 成員

| 變數 | 型別 | 說明 |
|------|------|------|
| `m_TriggerType` | `TriggerType` | 觸發類型 |
| `m_Duration` | `int` | 需要維持的幀數 |
| `m_IsTriggered` | `bool` | 條件是否已觸發 |
| `m_Timer` | `int` | 當前倒數計時 |
