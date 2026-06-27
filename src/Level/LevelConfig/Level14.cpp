#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/ShapeToAnythingContactPass.hpp"
#include <vector>

LevelConfig LevelConfig_14() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 2;
    data.targetText = "Make an object touch the red area";

    // Red sensor area at the top
    auto redArea = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(590.0F, 90.0F),
        glm::vec2(0.0F, 0.0F),
        0.0F,
        GameWorld::ShapeColor::Red,
        true,
        false,
        true);

    auto redAreaComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{redArea},
        GameWorld::BodyType::STATIC,
        glm::vec2(0.0F, 250.0F));

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary, redAreaComp};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passConditions = {
        std::make_shared<ShapeToAnythingContactPass>(
            redArea->Getb2ShapeId(),
            TriggerType::TOUCHING,
            3)
    };

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_14, LevelConfig_14);
    }
};
static Register reg;
}  // namespace
