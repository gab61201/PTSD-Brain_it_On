#include "Constants.hpp"
#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/ShapeToAnythingContactPass.hpp"

LevelConfig LevelConfig_6() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 2;
    data.targetText = "Place an object inside the container";

    auto boundary = std::make_shared<GameWorld::Boundary>();

    // Forbidden area: right half of boundary
    auto forbiddenShape = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(300.0F, 600.0F),
        glm::vec2(150.0F, 0.0F),
        0.0F,
        GameWorld::ShapeColor::Transparent,
        true,
        false,
        true);
    auto forbiddenComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{forbiddenShape},
        GameWorld::BodyType::STATIC,
        glm::vec2(0.0F, 0.0F));

    // Level 5 container on right half ground
    auto bottom = std::make_shared<GameWorld::Rectangle>(glm::vec2(160.0F, 20.0F), glm::vec2(0.0F, -90.0F));
    auto leftWall = std::make_shared<GameWorld::Rectangle>(glm::vec2(20.0F, 180.0F), glm::vec2(-85.0F, 0.0F), glm::radians(10.0F));
    auto rightWall = std::make_shared<GameWorld::Rectangle>(glm::vec2(20.0F, 180.0F), glm::vec2(85.0F, 0.0F), glm::radians(-10.0F));
    auto sensor = std::make_shared<GameWorld::Rectangle>(glm::vec2(160.0F, 180.0F), glm::vec2(0.0F, 0.0F), 0.0F, GameWorld::ShapeColor::Transparent, true, false);
    auto containerComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{bottom, leftWall, rightWall, sensor},
        GameWorld::BodyType::STATIC,
        glm::vec2(150.0F, -190.0F));

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary, forbiddenComp, containerComp};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passConditions = {
        std::make_shared<ShapeToAnythingContactPass>(
            sensor->Getb2ShapeId(),
            TriggerType::TOUCHING,
            3)
    };

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
