#include "Level/LevelData.hpp"
#include "Level/PassCondition.hpp"
#include "Util/Text.hpp"

// TESTING
static LevelData LevelData_1() {
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

    // ==========================================
    // 2. 建立「動態掉落的方塊」
    // ==========================================
    auto boxPart = std::make_shared<GameWorld::BaseObject>(
        GameWorld::ShapeType::RECTANGLE,
        glm::vec2(60.0F, 60.0F),  // 60x60 像素的方塊
        glm::vec2(0.0F, 0.0F));
    auto boxComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::BaseObject>>{boxPart},
        GameWorld::BodyType::DYNAMIC,  // 動態剛體 (會掉落)
        glm::vec2(0.0F, 200.0F)        // 絕對位置：放在半空中
    );

    // ==========================================
    // 3. 建立「動態掉落的圓球」 (用來測試滾動與旋轉)
    // ==========================================
    auto circlePart = std::make_shared<GameWorld::BaseObject>(
        GameWorld::ShapeType::CIRCLE,
        glm::vec2(50.0F, 50.0F),  // 直徑 50 像素 (半徑 25)
        glm::vec2(0.0F, 0.0F));
    auto circleComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::BaseObject>>{circlePart},
        GameWorld::BodyType::DYNAMIC,  // 動態剛體 (會掉落)
        glm::vec2(15.0F, 400.0F)       // 絕對位置：放在方塊正上方再稍微偏右，製造不平衡的撞擊！
    );

    // ==========================================
    // 4. 將所有組合件打包，並初始化物理世界
    // ==========================================
    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        floorComp, boxComp, circleComp};

    PassCondition* passCondition = new PassCondition(boxPart, circlePart, Condition::TOUCHING, 0);
    // 實例化物理世界 (建構子會自動將這些 objects 透過 AttachToWorld 掛載到 Box2D)
    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, passCondition);

    // 回傳設定好的關卡資料
    return data;
}

static LevelData LevelData_2() {
    return LevelData{};
}

static LevelData LevelData_3() {
    return LevelData{};
}

static LevelData LevelData_4() {
    return LevelData{};
}

static LevelData LevelData_5() {
    return LevelData{};
}

LevelData GetLevelData(LevelId levelId) {
    static constexpr LevelData (*level_funcs[])() = {
        LevelData_1,
        LevelData_2,
        LevelData_3,
        LevelData_4,
        LevelData_5};

    auto index = static_cast<std::size_t>(levelId);
    if (index < std::size(level_funcs)) {
        return level_funcs[index]();
    }

    return LevelData{};
}
