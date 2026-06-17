#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/ShapeToShapeContactPass.hpp"
#include "Level/PassCondition/ShapeToAnythingContactPass.hpp"

LevelConfig LevelConfig_13() {
    LevelConfig data;
    data.timeout = 8.0F;
    data.strokeLimit = 2;
    data.targetText = "Lift the ball off the ground";

    // 建立圓球 (動態，置於地上)
    auto ballPart = std::make_shared<GameWorld::Circle>(50.0f, glm::vec2(0.0F, 10.0F), GameWorld::ShapeColor::Orange);
    auto ballComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{ballPart},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, -275.0F));

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary, ballComp};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passConditions = {
        std::make_shared<ShapeToShapeContactPass>(
            ballPart->Getb2ShapeId(),
            boundary->GetBottomWall().Getb2ShapeId(),
            TriggerType::SEPARATED, 3)
    };

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_13, LevelConfig_13);
    }
};
static Register reg;
}  // namespace
