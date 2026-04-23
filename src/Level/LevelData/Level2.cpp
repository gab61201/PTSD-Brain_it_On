#include "Level/LevelData.hpp"
#include "Level/PassCondition/OneToOneContactPass.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "GameWorld/CompositeObject/Boundary.hpp"

LevelData LevelData_2() {
    LevelData data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Make the ball hit the left wall";

    // 3. 建立橘色小球
    auto ballPart = std::make_shared<GameWorld::Circle>(
        30.0f,
        glm::vec2(0.0F, 0.0F)
    );
    auto ballComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{ballPart},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, 200.0F)
    );

    auto boundary = std::make_shared<GameWorld::Boundary>(-300.0F, 300.0F, -300.0F, 300.0F);

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        ballComp, boundary};

    // 過關條件：小球碰到邊界的左牆 (index 0)
    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passCondition = std::make_shared<OneToOneContactPass>(ballPart->Getb2ShapeId(), boundary->GetShapes()[0]->Getb2ShapeId(), TriggerType::TOUCHING, 0);

    return data;
}

namespace {
    struct Register {
        Register() {
            RegisterLevel(LevelId::LEVEL_2, LevelData_2);
        }
    };

    static Register reg;
}
