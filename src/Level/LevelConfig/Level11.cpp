#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/CompositeObject/MagnetObject.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/ShapeToShapeContactPass.hpp"
#include <vector>

LevelConfig LevelConfig_11() {
    LevelConfig data;
    data.timeout = 5.0F;
    data.strokeLimit = 1;
    data.targetText = "Make the magnets touch";

    // 1. Platform in the middle
    auto platformShape = std::make_shared<GameWorld::Rectangle>(glm::vec2(300.0F, 10.0F), glm::vec2(0.0F, 0.0F));
    auto platformComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{platformShape},
        GameWorld::BodyType::STATIC,
        glm::vec2(0.0F, -50.0F));

    // 2. Positive Magnet (Blue)
    auto positiveShape = std::make_shared<GameWorld::Circle>(40.0F, glm::vec2(0.0F, 0.0F), GameWorld::ShapeColor::PositiveMagnet);
    auto positiveMagnet = std::make_shared<GameWorld::MagnetObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{positiveShape},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(-120.0F, -25.0F),
        0.0F,
        1.0F);

    // 3. Negative Magnet (Red)
    auto negativeShape = std::make_shared<GameWorld::Circle>(40.0F, glm::vec2(0.0F, 0.0F), GameWorld::ShapeColor::NegativeMagnet);
    auto negativeMagnet = std::make_shared<GameWorld::MagnetObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{negativeShape},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(120.0F, -25.0F),
        0.0F,
        -1.0F);

    // 4. Boundary
    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        boundary, platformComp, positiveMagnet, negativeMagnet};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);

    // 5. Pass Condition
    data.passConditions = {
        std::make_shared<ShapeToShapeContactPass>(
            positiveShape->Getb2ShapeId(),
            negativeShape->Getb2ShapeId(),
            TriggerType::TOUCHING, 0)
    };

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
