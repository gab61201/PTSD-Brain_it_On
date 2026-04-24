#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/OneToOneContactPass.hpp"

LevelData LevelData_3() {
    LevelData data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Tilt the shape to the right";

    // 2. Seesaw Shape
    auto bar = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(400.0F, 20.0F),
        glm::vec2(0.0F, 0.0F));
    auto pivot1 = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(20.0F, 100.0F),
        glm::vec2(0.0F, -60.0F)  // centered
    );
    // Add specific shapes for the tips to check for floor contact and look symmetrical
    auto leftTip = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(20.0F, 20.0F),
        glm::vec2(-190.0F, -20.0F));
    auto rightTip = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(20.0F, 20.0F),
        glm::vec2(190.0F, -20.0F));

    // We start it a bit high, slightly tilted left (rotation = 0.1) so it falls to the left side
    auto seesawComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{bar, pivot1, leftTip, rightTip},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, -180.0F),
        0.33F);

    auto boundary = std::make_shared<GameWorld::Boundary>(-300.0F, 300.0F, -300.0F, 300.0F);

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        seesawComp, boundary};

    // Pass condition: rightTip touches the floor (boundary index 3 is bottom wall)
    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passCondition = std::make_shared<OneToOneContactPass>(rightTip->Getb2ShapeId(), boundary->GetShapes()[3]->Getb2ShapeId(), TriggerType::TOUCHING, 0);

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_3, LevelData_3);
    }
};

static Register reg;
}  // namespace
