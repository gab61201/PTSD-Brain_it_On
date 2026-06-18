#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/CompositeObject/MagnetObject.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/ShapeToShapeContactPass.hpp"
#include <vector>

LevelConfig LevelConfig_12() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Separate the magnets";

    // 1. Positive Magnet (Blue +)
    auto positiveShape = std::make_shared<GameWorld::Circle>(50.0F, glm::vec2(0.0F, 0.0F), GameWorld::ShapeColor::PositiveMagnet);
    auto positiveMagnet = std::make_shared<GameWorld::MagnetObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{positiveShape},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(-25.0F, -265.0F),
        0.0F,
        4.0F);

    // 2. Negative Magnet (Red -)
    auto negativeShape = std::make_shared<GameWorld::Circle>(50.0F, glm::vec2(0.0F, 0.0F), GameWorld::ShapeColor::NegativeMagnet);
    auto negativeMagnet = std::make_shared<GameWorld::MagnetObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{negativeShape},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(25.0F, -265.0F),
        0.0F,
        -4.0F);

    // 3. Boundary
    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary, positiveMagnet, negativeMagnet};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);

    // 4. Pass Condition: magnets are separated
    data.passConditions = {
        std::make_shared<ShapeToShapeContactPass>(
            positiveShape->Getb2ShapeId(),
            negativeShape->Getb2ShapeId(),
            TriggerType::SEPARATED, 3)
    };

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_12, LevelConfig_12);
    }
};
static Register reg;
}  // namespace
