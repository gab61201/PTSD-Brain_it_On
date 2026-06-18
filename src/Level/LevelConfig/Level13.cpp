#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/CountObjectsInSensorPass.hpp"
#include <vector>

LevelConfig LevelConfig_13() {
    LevelConfig data;
    data.timeout = 8.0F;
    data.strokeLimit = 1;
    data.targetText = "Get the ball out of the container";

  // 1. X-shaped container (two crossed bars)
    auto barShape1 = std::make_shared<GameWorld::Rectangle>(glm::vec2(12.0F, 320.0F), glm::vec2(20.0F, 0.0F), glm::radians(18.0F));
    auto barShape2 = std::make_shared<GameWorld::Rectangle>(glm::vec2(12.0F, 320.0F), glm::vec2(-20.0F, 0.0F), glm::radians(-18.0F));
    auto sensorShape = std::make_shared<GameWorld::Rectangle>(glm::vec2(50.0F, 80.0F), glm::vec2(0.0F, 110.0F), 0.0F, GameWorld::ShapeColor::Transparent, true, false);

    auto containerComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{barShape1, barShape2, sensorShape},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, -135.0F));

    // 2. Orange ball trapped in the V
    auto ballShape = std::make_shared<GameWorld::Circle>(15.0F, glm::vec2(0.0F, 0.0F), GameWorld::ShapeColor::Orange);
    auto ballComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{ballShape},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, -25.0F));

    // 3. Boundary
    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        boundary, containerComp, ballComp};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);

    // 4. Pass Condition: ball outside sensor for 3 seconds
    data.passConditions = {
        std::make_shared<CountObjectsInSensorPass>(
            std::vector<b2ShapeId>{ballShape->Getb2ShapeId()},
            sensorShape->Getb2ShapeId(),
            0, 3)
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
