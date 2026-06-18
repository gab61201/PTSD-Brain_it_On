#include "Constants.hpp"
#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/ShapeToAnythingContactPass.hpp"

LevelConfig LevelConfig_7() {
    LevelConfig data;
    data.timeout = 8.0F;
    data.strokeLimit = 2;
    data.targetText = "Place an object inside the container";

    auto boundary = std::make_shared<GameWorld::Boundary>();

    // Gray vertical wall separating left (blue) from right (red) zone
    auto wallShape = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(20.0F, 400.0F),
        glm::vec2(0.0F, 0.0F),
        0.0F,
        GameWorld::ShapeColor::Gray);
    auto wallComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{wallShape},
        GameWorld::BodyType::STATIC,
        glm::vec2(150.0F, -100.0F));

    auto forbiddenShape = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(130.0F, 600.0F),
        glm::vec2(0.0F, 0.0F),
        0.0F,
        GameWorld::ShapeColor::Transparent,
        true,
        false,
        true);
    auto forbiddenComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{forbiddenShape},
        GameWorld::BodyType::STATIC,
        glm::vec2(225.0F, 0.0F));

    // U-shaped container at bottom of red zone
    auto bottom = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(110.0F, 15.0F),
        glm::vec2(0.0F, -65.0F));
    auto leftWall = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(15.0F, 140.0F),
        glm::vec2(-50.0F, 0.0F));
    auto rightWall = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(15.0F, 140.0F),
        glm::vec2(50.0F, 0.0F));
    auto sensor = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(100.0F, 120.0F),
        glm::vec2(0.0F, 0.0F),
        0.0F,
        GameWorld::ShapeColor::Transparent,
        true,
        false);
    auto containerComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{bottom, leftWall, rightWall, sensor},
        GameWorld::BodyType::STATIC,
        glm::vec2(225.0F, -215.0F));

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary, wallComp, containerComp, forbiddenComp};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passConditions = {
        std::make_shared<ShapeToAnythingContactPass>(
            sensor->Getb2ShapeId(),
            TriggerType::TOUCHING,
            0)
    };

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_7, LevelConfig_7);
    }
};
static Register reg;
}  // namespace
