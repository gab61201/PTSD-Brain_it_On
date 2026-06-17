#include "GameWorld/CompositeObject/Boundary.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/ShapeToShapeContactPass.hpp"
#include "Level/PassCondition/ShapeToAnythingContactPass.hpp"

LevelConfig LevelConfig_6() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Draw a shape";

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passCondition = std::make_shared<ShapeToAnythingContactPass>(
        boundary->GetBottomWall().Getb2ShapeId(),
        TriggerType::TOUCHING, 0);

    b2Shape_EnableContactEvents(boundary->GetBottomWall().Getb2ShapeId(), true);

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_6, LevelConfig_6);
    }
};
static Register reg;
}  // namespace
