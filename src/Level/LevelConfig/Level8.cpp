#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/OneToOneContactPass.hpp"

LevelConfig LevelConfig_8() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Tilt the shape to the right";

    // 1. 建立蹺蹺板 (Seesaw)
    auto bar = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(400.0F, 20.0F),
        glm::vec2(0.0F, 0.0F));
    auto pivot1 = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(20.0F, 100.0F),
        glm::vec2(0.0F, -60.0F)); // 置中
    auto leftTip = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(20.0F, 20.0F),
        glm::vec2(-190.0F, -20.0F));
    auto rightTip = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(20.0F, 20.0F),
        glm::vec2(190.0F, -20.0F));

    // 起始位置偏高且稍微偏左傾斜 (角度 0.33 rad)，使其先靠在左邊
    auto seesawComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{bar, pivot1, leftTip, rightTip},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, -180.0F),
        0.33F);

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        seesawComp, boundary};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passCondition = std::make_shared<OneToOneContactPass>(
        rightTip->Getb2ShapeId(),
        boundary->GetBottomWall().Getb2ShapeId(),
        TriggerType::TOUCHING, 0);

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_8, LevelConfig_8);
    }
};
static Register reg;
}  // namespace
