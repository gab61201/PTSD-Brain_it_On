#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/CountObjectsInSensorPass.hpp"

LevelConfig LevelConfig_9() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Get the ball out of the container";

    // 1. Create the container (dynamic, shallow U-shape made of 5 segments, R=100)
    glm::vec2 size(60.0F, 15.0F);
    auto p1 = std::make_shared<GameWorld::Rectangle>(size, glm::vec2(-86.6F, -50.0F), glm::radians(120.0F));
    auto p2 = std::make_shared<GameWorld::Rectangle>(size, glm::vec2(-50.0F, -86.6F), glm::radians(150.0F));
    auto p3 = std::make_shared<GameWorld::Rectangle>(size, glm::vec2(0.0F, -100.0F), glm::radians(180.0F));
    auto p4 = std::make_shared<GameWorld::Rectangle>(size, glm::vec2(50.0F, -86.6F), glm::radians(210.0F));
    auto p5 = std::make_shared<GameWorld::Rectangle>(size, glm::vec2(86.6F, -50.0F), glm::radians(240.0F));

    // Sensor bound to container — covers the interior area
    auto sensorShape = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(150.0F, 80.0F),
        glm::vec2(0.0F, -55.0F),
        0.0F,
        GameWorld::ShapeColor::Transparent,
        true,
        false);

    auto containerComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{p1, p2, p3, p4, p5, sensorShape},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, -180.0F));

    // 2. Create the ball (dynamic, inside the container)
    auto ballPart = std::make_shared<GameWorld::Circle>(40.0F, glm::vec2(0.0F, 0.0F), GameWorld::ShapeColor::Orange);
    auto ballComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{ballPart},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, -250.0F));

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary,
        containerComp, ballComp};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);

    // Pass condition: ball outside sensor for 3 seconds
    data.passConditions = {
        std::make_shared<CountObjectsInSensorPass>(
            std::vector<b2ShapeId>{ballPart->Getb2ShapeId()},
            sensorShape->Getb2ShapeId(),
            0, 3)
    };

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_9, LevelConfig_9);
    }
};
static Register reg;
}  // namespace
