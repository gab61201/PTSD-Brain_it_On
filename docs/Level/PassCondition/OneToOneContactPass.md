# OneToOneContactPass

**標頭檔**：[`include/Level/PassCondition/OneToOneContactPass.hpp`](../../../include/Level/PassCondition/OneToOneContactPass.hpp)

繼承 [`PassCondition`](PassCondition.md)。一對一碰撞過關條件：指定的兩個形狀接觸 / 分離時觸發。

#### 建構子

```cpp
// 指定兩個特定形狀
OneToOneContactPass(
    b2ShapeId shapeA,
    b2ShapeId shapeB,
    TriggerType triggerType,
    int duration              // 需維持接觸的幀數
);

// 單一形狀與任意物體接觸
OneToOneContactPass(
    b2ShapeId shape,
    TriggerType triggerType,
    int duration
);
```
