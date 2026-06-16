#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/AnyToOneContactPass.hpp"

LevelConfig LevelConfig_15() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Remove the cap";

    // 1. 建立錐形基底 (靜態，階梯金字塔狀以近似三角形)
    auto block1 = std::make_shared<GameWorld::Rectangle>(glm::vec2(200.0F, 50.0F), glm::vec2(0.0F, -75.0F));
    auto block2 = std::make_shared<GameWorld::Rectangle>(glm::vec2(150.0F, 50.0F), glm::vec2(0.0F, -25.0F));
    auto block3 = std::make_shared<GameWorld::Rectangle>(glm::vec2(100.0F, 50.0F), glm::vec2(0.0F, 25.0F));
    auto block4 = std::make_shared<GameWorld::Rectangle>(glm::vec2(50.0F, 50.0F), glm::vec2(0.0F, 75.0F)); // 頂部平台

    auto baseComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{block1, block2, block3, block4},
        GameWorld::BodyType::STATIC,
        glm::vec2(0.0F, -200.0F));

    // 2. 建立蓋子 (動態)
    auto capTop = std::make_shared<GameWorld::Rectangle>(glm::vec2(60.0F, 15.0F), glm::vec2(0.0F, 20.0F));
    auto capLeft = std::make_shared<GameWorld::Rectangle>(glm::vec2(10.0F, 30.0F), glm::vec2(-25.0F, 5.0F));
    auto capRight = std::make_shared<GameWorld::Rectangle>(glm::vec2(10.0F, 30.0F), glm::vec2(25.0F, 5.0F));

    auto capComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{capTop, capLeft, capRight},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, -90.0F));

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        baseComp, capComp, boundary};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passCondition = std::make_shared<AnyToOneContactPass>(
        std::vector<b2ShapeId>{capLeft->Getb2ShapeId(), capRight->Getb2ShapeId()},
        block4->Getb2ShapeId(),
        TriggerType::SEPARATED, 3);

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_15, LevelConfig_15);
    }
};
static Register reg;
}  // namespace
