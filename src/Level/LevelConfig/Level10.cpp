#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/CountObjectsInSensorPass.hpp"
#include <algorithm>
#include <vector>

LevelConfig LevelConfig_10() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Place only two balls in the container";

    // 1. Platform on the left
    auto platform = std::make_shared<GameWorld::Rectangle>(glm::vec2(200.0F, 15.0F), glm::vec2(0.0F, 0.0F));
    auto platformComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{platform},
        GameWorld::BodyType::STATIC,
        glm::vec2(-200.0F, -50.0F));

    // 2. Three balls
    auto ballShape1 = std::make_shared<GameWorld::Circle>(30.0F, glm::vec2(0.0F, 0.0F), GameWorld::ShapeColor::Orange);
    auto ballComp1 = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{ballShape1},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(-240.0F, -27.0F));

    auto ballShape2 = std::make_shared<GameWorld::Circle>(30.0F, glm::vec2(0.0F, 0.0F), GameWorld::ShapeColor::Orange);
    auto ballComp2 = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{ballShape2},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(-200.0F, -27.0F));

    auto ballShape3 = std::make_shared<GameWorld::Circle>(30.0F, glm::vec2(0.0F, 0.0F), GameWorld::ShapeColor::Orange);
    auto ballComp3 = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{ballShape3},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(-160.0F, -27.0F));

    // 3. Container
    auto bottom = std::make_shared<GameWorld::Rectangle>(glm::vec2(160.0F, 20.0F), glm::vec2(0.0F, -90.0F));
    auto leftWall = std::make_shared<GameWorld::Rectangle>(glm::vec2(20.0F, 180.0F), glm::vec2(-85.0F, 0.0F), glm::radians(10.0F));
    auto rightWall = std::make_shared<GameWorld::Rectangle>(glm::vec2(20.0F, 180.0F), glm::vec2(85.0F, 0.0F), glm::radians(-10.0F));
    auto sensor = std::make_shared<GameWorld::Rectangle>(glm::vec2(120.0F, 160.0F), glm::vec2(0.0F, 0.0F), 0.0F, GameWorld::ShapeColor::White, true, false);

    auto containerComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{bottom, leftWall, rightWall, sensor},
        GameWorld::BodyType::STATIC,
        glm::vec2(0.0F, -200.0F));

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary, 
        platformComp, ballComp1, ballComp2, ballComp3, containerComp};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);

    std::vector<b2ShapeId> balls = {
        ballShape1->Getb2ShapeId(),
        ballShape2->Getb2ShapeId(),
        ballShape3->Getb2ShapeId()
    };

    data.passConditions = {
        std::make_shared<CountObjectsInSensorPass>(
            balls,
            sensor->Getb2ShapeId(),
            2)
    };

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_10, LevelConfig_10);
    }
};
static Register reg;
}  // namespace
