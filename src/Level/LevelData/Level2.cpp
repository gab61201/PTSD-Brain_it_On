#include "Level/LevelData.hpp"
#include "Level/PassCondition/OneToOneContactPass.hpp"

LevelData LevelData_2() {
    LevelData data;
    data.timeout = 60.0F;  // 設定通關時間為 60 秒

    // ==========================================
    // 1. 建立「靜態地板」 (不會動，用來接住掉下來的物體)
    // ==========================================
    auto floorPart = std::make_shared<GameWorld::BaseObject>(
        GameWorld::ShapeType::RECTANGLE,
        glm::vec2(800.0F, 50.0F),  // 寬 800 像素，高 50 像素
        glm::vec2(0.0F, 0.0F)      // 相對於父物件中心的偏移量為 0
    );
    auto floorComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::BaseObject>>{floorPart},
        GameWorld::BodyType::STATIC,  // 靜態剛體 (不受重力影響)
        glm::vec2(0.0F, -300.0F)      // 絕對位置：放在畫面偏下方 (依據你的座標系微調)
    );
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
    // 第二個磁鐵（負極，會與第一個磁鐵相吸）
    auto magnetPart2 = std::make_shared<GameWorld::BaseObject>(
        GameWorld::ShapeType::CIRCLE,
        glm::vec2(50.0F, 50.0F),
        glm::vec2(0.0F, 0.0F));
    auto magnetComp2 = std::make_shared<GameWorld::MagnetObject>(
        std::vector<std::shared_ptr<GameWorld::BaseObject>>{magnetPart2},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(200.0F, 0.0F),  // 放在右邊
        0.0F,
        -30.0F  // 負極：與第一個磁鐵 (+30) 異極相吸
    );

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        floorComp, magnetComp, magnetComp2};

    // 實例化物理世界 (建構子會自動將這些 objects 透過 AttachToWorld 掛載到 Box2D)
    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, new OneToOneContactPass(magnetPart, magnetPart2, TriggerType::TOUCHING, 3));

    // 回傳設定好的關卡資料
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
