#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/OneToOneContactPass.hpp"

LevelConfig LevelConfig_5() {
    LevelConfig data;
    data.timeout = 60.0F;
    data.strokeLimit = 3;
    data.targetText = "Connect the shapes";

    auto boxPart = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(60.0F, 60.0F),
        glm::vec2(0.0F, 0.0F));
    auto boxComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{boxPart},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(-150.0F, 0.0F)
    );

    auto circlePart = std::make_shared<GameWorld::Circle>(
        50.0f,
        glm::vec2(0.0F, 0.0F),
        GameWorld::ShapeColor::Orange);
    auto circleComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{circlePart},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(150.0F, 0.0F)
    );

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary, 
        boxComp, circleComp};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passCondition = std::make_shared<OneToOneContactPass>(
        boxPart->Getb2ShapeId(),
        circlePart->Getb2ShapeId(),
        TriggerType::TOUCHING, 3);

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_5, LevelConfig_5);
    }
};
static Register reg;
}  // namespace
