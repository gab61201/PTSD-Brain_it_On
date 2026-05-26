# 新增關卡範例

以下示範如何在 `src/Level/LevelConfig/` 中新增一個關卡：

```cpp
#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/OneToOneContactPass.hpp"

LevelConfig LevelConfig_N() {
    LevelConfig data;
    data.timeout = 45.0F;
    data.strokeLimit = 2;

    // 建立物件
    auto rect = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(80.0F, 80.0F), glm::vec2(0.0F, 0.0F));
    auto rectObj = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{rect},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, 200.0F));

    // 建立邊界
    auto boundary = std::make_shared<GameWorld::Boundary>(
        -300.0F, 300.0F, -300.0F, 300.0F);

    // 組裝物理世界
    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        rectObj, boundary};
    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);

    // 設定過關條件
    data.passCondition = std::make_shared<OneToOneContactPass>(
        rect->Getb2ShapeId(), TriggerType::TOUCHING, 3);

    return data;
}

// 自動註冊
namespace {
struct Register {
    Register() { RegisterLevel(LevelId::LEVEL_N, LevelConfig_N); }
};
static Register reg;
}
```

> **注意**：需要先在 `LevelId` 列舉中加入 `LEVEL_N`。
