#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/CompositeObject/MagnetObject.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/OneToOneContactPass.hpp"

LevelConfig LevelConfig_4() {
    LevelConfig data;
    data.timeout = 60.0F;
    data.strokeLimit = 3;
    data.targetText = "Separate the magnets";

    auto magnetPart1 = std::make_shared<GameWorld::Circle>(
        60.0f, glm::vec2(0.0F, 0.0F));
    auto magnetComp1 = std::make_shared<GameWorld::MagnetObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{magnetPart1},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(-30.0F, -260.0F),
        0.0F,
        -5.0F);

    auto magnetPart2 = std::make_shared<GameWorld::Circle>(
        60.0f, glm::vec2(0.0F, 0.0F));
    auto magnetComp2 = std::make_shared<GameWorld::MagnetObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{magnetPart2},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(30.0F, -260.0F),
        0.0F,
        5.0F);

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary, 
        magnetComp1, magnetComp2};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passCondition = std::make_shared<OneToOneContactPass>(
        magnetPart1->Getb2ShapeId(),
        magnetPart2->Getb2ShapeId(),
        TriggerType::SEPARATED, 3);

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_4, LevelConfig_4);
    }
};
static Register reg;
}  // namespace
