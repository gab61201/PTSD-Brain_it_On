#include "GameWorld/MagnetObject.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/OneToOneContactPass.hpp"

LevelData LevelData_3() {
    LevelData data;
    data.timeout = 60.0F;  // 原本磁鐵關卡的設定
    data.strokeLimit = 3;
    data.targetText = "Magnet Test Level";

    // 1. 建立「靜態地板」
    auto floorPart = std::make_shared<GameWorld::BaseObject>(
        GameWorld::ShapeType::RECTANGLE,
        glm::vec2(800.0F, 50.0F),
        glm::vec2(0.0F, 0.0F));
    auto floorComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::BaseObject>>{floorPart},
        GameWorld::BodyType::STATIC,
        glm::vec2(0.0F, -300.0F));

    // 磁鐵測試
    auto magnetPart = std::make_shared<GameWorld::BaseObject>(
        GameWorld::ShapeType::CIRCLE,
        glm::vec2(50.0F, 50.0F),
        glm::vec2(0.0F, 0.0F));
    auto magnetComp = std::make_shared<GameWorld::MagnetObject>(
        std::vector<std::shared_ptr<GameWorld::BaseObject>>{magnetPart},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, 0.0F),
        0.0F,
        30.0F);

    // 第二個磁鐵（負極）
    auto magnetPart2 = std::make_shared<GameWorld::BaseObject>(
        GameWorld::ShapeType::CIRCLE,
        glm::vec2(50.0F, 50.0F),
        glm::vec2(0.0F, 0.0F));
    auto magnetComp2 = std::make_shared<GameWorld::MagnetObject>(
        std::vector<std::shared_ptr<GameWorld::BaseObject>>{magnetPart2},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(200.0F, 0.0F),
        0.0F,
        -30.0F);

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        floorComp, magnetComp, magnetComp2};

    // 實例化物理世界
    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, new OneToOneContactPass(magnetPart, magnetPart2, TriggerType::TOUCHING, 3));

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
