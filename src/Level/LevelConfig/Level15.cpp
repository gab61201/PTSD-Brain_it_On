#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/ShapeToShapeContactPass.hpp"
#include <vector>

LevelConfig LevelConfig_15() {
    LevelConfig data;
    data.timeout = 15.0F;
    data.strokeLimit = 1;
    data.targetText = "Make the ball touch the ground";

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects;

    // 1. Boundary
    auto boundary = std::make_shared<GameWorld::Boundary>();
    objects.push_back(boundary);

    // Universal Domino Size
    const glm::vec2 DOMINO_SIZE(70.0F, 10.0F);
    
    // Add small gaps to prevent Box2D from resolving explosive initial overlaps
    const float GAP_X = 0.2F;
    const float GAP_Y = 0.1F;
    
    // SPACING is the distance between the vertical pillars.
    const float SPACING = DOMINO_SIZE.x / 2.0F + GAP_X / 2.0F; 
    
    const float GROUND_Y = -290.0F; // Boundary bottom wall top surface
    const float ROT_VERTICAL = glm::half_pi<float>();
    const float ROT_HORIZONTAL = 0.0F;

    float current_y = GROUND_Y;

    // Layer 0: 11 vertical pillars (i = -5 to 5)
    float l0_y = current_y + DOMINO_SIZE.x / 2.0F + GAP_Y;
    for (int i = -5; i <= 5; ++i) {
        auto rect = std::make_shared<GameWorld::Rectangle>(DOMINO_SIZE, glm::vec2(0.0F, 0.0F), ROT_VERTICAL);
        auto comp = std::make_shared<GameWorld::CompositeObject>(
            std::vector<std::shared_ptr<GameWorld::Shape>>{rect},
            GameWorld::BodyType::DYNAMIC,
            glm::vec2(i * SPACING, l0_y)
        );
        objects.push_back(comp);
    }
    current_y = l0_y + DOMINO_SIZE.x / 2.0F;

    // Layer 1: 5 horizontal dominoes (centers at i = -4, -2, 0, 2, 4)
    float l1_y = current_y + DOMINO_SIZE.y / 2.0F + GAP_Y;
    for (int i : {-4, -2, 0, 2, 4}) {
        auto rect = std::make_shared<GameWorld::Rectangle>(DOMINO_SIZE, glm::vec2(0.0F, 0.0F), ROT_HORIZONTAL);
        auto comp = std::make_shared<GameWorld::CompositeObject>(
            std::vector<std::shared_ptr<GameWorld::Shape>>{rect},
            GameWorld::BodyType::DYNAMIC,
            glm::vec2(i * SPACING, l1_y)
        );
        objects.push_back(comp);
    }
    current_y = l1_y + DOMINO_SIZE.y / 2.0F;

    // Layer 2: 9 vertical pillars (i = -4 to 4)
    float l2_y = current_y + DOMINO_SIZE.x / 2.0F + GAP_Y;
    for (int i = -4; i <= 4; ++i) {
        auto rect = std::make_shared<GameWorld::Rectangle>(DOMINO_SIZE, glm::vec2(0.0F, 0.0F), ROT_VERTICAL);
        auto comp = std::make_shared<GameWorld::CompositeObject>(
            std::vector<std::shared_ptr<GameWorld::Shape>>{rect},
            GameWorld::BodyType::DYNAMIC,
            glm::vec2(i * SPACING, l2_y)
        );
        objects.push_back(comp);
    }
    current_y = l2_y + DOMINO_SIZE.x / 2.0F;

    // Layer 3: 3 horizontal dominoes (centers at i = -2, 0, 2)
    float l3_y = current_y + DOMINO_SIZE.y / 2.0F + GAP_Y;
    for (int i : {-2, 0, 2}) {
        auto rect = std::make_shared<GameWorld::Rectangle>(DOMINO_SIZE, glm::vec2(0.0F, 0.0F), ROT_HORIZONTAL);
        auto comp = std::make_shared<GameWorld::CompositeObject>(
            std::vector<std::shared_ptr<GameWorld::Shape>>{rect},
            GameWorld::BodyType::DYNAMIC,
            glm::vec2(i * SPACING, l3_y)
        );
        objects.push_back(comp);
    }
    current_y = l3_y + DOMINO_SIZE.y / 2.0F;

    // Layer 4: 3 spaced-out vertical pillars (i = -2, 0, 2)
    float l4_y = current_y + DOMINO_SIZE.x / 2.0F + GAP_Y;
    for (int i : {-2, 0, 2}) {
        auto rect = std::make_shared<GameWorld::Rectangle>(DOMINO_SIZE, glm::vec2(0.0F, 0.0F), ROT_VERTICAL);
        auto comp = std::make_shared<GameWorld::CompositeObject>(
            std::vector<std::shared_ptr<GameWorld::Shape>>{rect},
            GameWorld::BodyType::DYNAMIC,
            glm::vec2(i * SPACING, l4_y)
        );
        objects.push_back(comp);
    }
    current_y = l4_y + DOMINO_SIZE.x / 2.0F;

    // Layer 5: 1 extra vertical domino right under the ball (to prop it up higher)
    float l5_y = current_y + DOMINO_SIZE.x / 2.0F + GAP_Y;
    auto extraRect = std::make_shared<GameWorld::Rectangle>(DOMINO_SIZE, glm::vec2(0.0F, 0.0F), ROT_VERTICAL);
    auto extraComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{extraRect},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, l5_y)
    );
    objects.push_back(extraComp);
    current_y = l5_y + DOMINO_SIZE.x / 2.0F;

    // Orange ball (rests on the extra pillar of Layer 5)
    float ball_radius = 15.0F;
    float ball_y = current_y + ball_radius + GAP_Y;
    auto ballShape = std::make_shared<GameWorld::Circle>(ball_radius, glm::vec2(0.0F, 0.0F), GameWorld::ShapeColor::Orange);
    auto ballComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{ballShape},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, ball_y)
    );
    objects.push_back(ballComp);

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);

    // Pass Condition: ball touches the ground
    data.passConditions = {
        std::make_shared<ShapeToShapeContactPass>(
            ballShape->Getb2ShapeId(),
            boundary->GetBottomWall().Getb2ShapeId(),
            TriggerType::TOUCHING, 3)
    };

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
