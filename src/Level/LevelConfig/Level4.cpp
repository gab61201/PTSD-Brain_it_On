#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/GroupToShapeContactPass.hpp"

LevelConfig LevelConfig_4() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Tip the glass onto the ground";

    auto leftWall = std::make_shared<GameWorld::Rectangle>(glm::vec2(15.0F, 120.0F), glm::vec2(-50.0F, 0.0F));
    auto rightWall = std::make_shared<GameWorld::Rectangle>(glm::vec2(15.0F, 120.0F), glm::vec2(50.0F, 0.0F));
    auto bottom = std::make_shared<GameWorld::Rectangle>(glm::vec2(115.0F, 15.0F), glm::vec2(0.0F, -60.0F));

    auto glassComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{leftWall, rightWall, bottom},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, -223.0F));

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary, glassComp};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passConditions = {
        std::make_shared<GroupToShapeContactPass>(
            std::vector<b2ShapeId>{leftWall->Getb2ShapeId(), rightWall->Getb2ShapeId()},
            boundary->GetBottomWall().Getb2ShapeId(),
            TriggerType::TOUCHING, 3)
    };

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
