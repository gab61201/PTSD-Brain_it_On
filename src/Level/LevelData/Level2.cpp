// #include "Level/LevelData.hpp"
// #include "Level/PassCondition/OneToOneContactPass.hpp"

// LevelData LevelData_2() {
//     LevelData data;
//     data.timeout = 10.0F;
//     data.targetText = "Make the ball hit the left wall";

//     // 1. 建立地板
//     auto floorPart = std::make_shared<GameWorld::BaseObject>(
//         GameWorld::ShapeType::RECTANGLE,
//         glm::vec2(800.0F, 50.0F),
//         glm::vec2(0.0F, 0.0F)
//     );
//     auto floorComp = std::make_shared<GameWorld::CompositeObject>(
//         std::vector<std::shared_ptr<GameWorld::BaseObject>>{floorPart},
//         GameWorld::BodyType::STATIC,
//         glm::vec2(0.0F, -350.0F)
//     );

//     // 2. 建立左側牆壁
//     auto leftWallPart = std::make_shared<GameWorld::BaseObject>(
//         GameWorld::ShapeType::RECTANGLE,
//         glm::vec2(50.0F, 600.0F),
//         glm::vec2(0.0F, 0.0F)
//     );
//     auto leftWallComp = std::make_shared<GameWorld::CompositeObject>(
//         std::vector<std::shared_ptr<GameWorld::BaseObject>>{leftWallPart},
//         GameWorld::BodyType::STATIC,
//         glm::vec2(-350.0F, 0.0F)
//     );

//     // 3. 建立橘色小球
//     auto ballPart = std::make_shared<GameWorld::BaseObject>(
//         GameWorld::ShapeType::CIRCLE,
//         glm::vec2(80.0F, 80.0F),
//         glm::vec2(0.0F, 0.0F)
//     );
//     auto ballComp = std::make_shared<GameWorld::CompositeObject>(
//         std::vector<std::shared_ptr<GameWorld::BaseObject>>{ballPart},
//         GameWorld::BodyType::DYNAMIC,
//         glm::vec2(0.0F, -280.0F)
//     );

//     std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
//         floorComp, leftWallComp, ballComp};

//     // 過關條件：小球碰到左牆
//     data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, new OneToOneContactPass(ballPart, leftWallPart, TriggerType::TOUCHING, 0));

//     return data;
// }

// namespace {
//     struct Register {
//         Register() {
//             RegisterLevel(LevelId::LEVEL_2, LevelData_2);
//         }
//     };

//     static Register reg;
// }
