#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/OneToOneContactPass.hpp"
#include "Level/PassCondition/PassCondition.hpp"

LevelConfig LevelConfig_11() {
    LevelConfig data;
    data.timeout = 20.0F;
    data.strokeLimit = 2;
    data.targetText = "Connect the shapes without entering the red zone";

    // 1. Box on the left
    auto boxShape = std::make_shared<GameWorld::Rectangle>(glm::vec2(50.0F, 50.0F), glm::vec2(0.0F, 0.0F));
    auto boxComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{boxShape},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(-200.0F, -50.0F));

    // 2. Circle on the right
    auto circleShape = std::make_shared<GameWorld::Circle>(50.0F, glm::vec2(0.0F, 0.0F));
    auto circleComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{circleShape},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(200.0F, -50.0F));

    // 3. Forbidden zone in the center (isSensor=true, isForbidden=true, outline=false)
    auto forbiddenZone = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(200.0F, 300.0F),
        glm::vec2(0.0F, 0.0F),
        0.0F,
        true,   // isSensor
        false,  // outline
        true    // isForbidden
    );
    auto forbiddenComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{forbiddenZone},
        GameWorld::BodyType::STATIC,
        glm::vec2(0.0F, 0.0F));

    // 4. Boundary
    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        boxComp, circleComp, forbiddenComp, boundary};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);

    data.passCondition = std::make_shared<OneToOneContactPass>(
        boxShape->Getb2ShapeId(),
        circleShape->Getb2ShapeId(),
        TriggerType::TOUCHING,
        3);

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_11, LevelConfig_11);
    }
};
static Register reg;
}  // namespace
