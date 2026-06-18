#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/ShapeToShapeContactPass.hpp"

LevelConfig LevelConfig_3() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Tilt the shape to the right";

    auto bar = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(400.0F, 20.0F),
        glm::vec2(0.0F, 0.0F));
    auto pivot = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(20.0F, 80.0F),
        glm::vec2(0.0F, -50.0F));
    auto leftTip = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(20.0F, 20.0F),
        glm::vec2(-190.0F, -20.0F));
    auto rightTip = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(20.0F, 20.0F),
        glm::vec2(190.0F, -20.0F));

    auto seesawComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{bar, pivot, leftTip, rightTip},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, -200.0F),
        0.3F);

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary, seesawComp};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passConditions = {
        std::make_shared<ShapeToShapeContactPass>(
            rightTip->Getb2ShapeId(),
            boundary->GetBottomWall().Getb2ShapeId(),
            TriggerType::TOUCHING, 0)
    };

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_3, LevelConfig_3);
    }
};

static Register reg;
}  // namespace
