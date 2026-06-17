#include "GameWorld/CompositeObject/Boundary.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/ShapeToAnythingContactPass.hpp"

LevelConfig LevelConfig_1() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Draw a shape";

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passConditions = {
        std::make_shared<ShapeToAnythingContactPass>(
            boundary->GetBottomWall().Getb2ShapeId(),
            TriggerType::TOUCHING, 0)
    };

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_1, LevelConfig_1);
    }
};

static Register reg;
}  // namespace
