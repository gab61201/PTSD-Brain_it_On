# PhysicalWorld

**標頭檔**：[`include/GameWorld/PhysicalWorld.hpp`](../../include/GameWorld/PhysicalWorld.hpp)

物理世界管理器，封裝 Box2D 世界並管理所有物理物件。

```cpp
PhysicalWorld(
    std::vector<std::shared_ptr<CompositeObject>> compositeObjects,
    std::shared_ptr<Boundary> boundary
);
~PhysicalWorld();
```

#### 方法

| 方法 | 簽章 | 說明 |
|------|------|------|
| `Start` | `void Start()` | 啟動物理模擬 |
| `Stop` | `void Stop()` | 停止物理模擬 |
| `DrawObject` | `void DrawObject(glm::vec2 position)` | 在指定位置開始 / 延續繪製物件 |
| `EndDrawing` | `void EndDrawing()` | 結束當前繪製 |
| `Update` | `void Update()` | 每幀更新物理步進與渲染 |
| `GetDrawnObjectCount` | `int GetDrawnObjectCount() const` | 取得已繪製的物件數量（筆畫數） |
| `GetContactEvents` | `b2ContactEvents GetContactEvents()` | 取得當前幀的碰撞事件 |
