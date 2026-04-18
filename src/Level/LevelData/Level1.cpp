#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Circle.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "GameWorld/Shape/Capsule.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/OneToOneContactPass.hpp"

LevelData LevelData_1() {
    LevelData data;
    data.timeout = 60.0F;  // 設定通關時間為 60 秒
    data.strokeLimit = 3;

    // ==========================================
    // 1. 建立「動態掉落的方塊」
    // ==========================================
    auto boxPart = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(60.0F, 60.0F),  // 60x60 像素的方塊
        glm::vec2(0.0F, 0.0F));
    auto boxComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{boxPart},
        GameWorld::BodyType::DYNAMIC,  // 動態剛體 (會掉落)
        glm::vec2(0.0F, 200.0F)        // 絕對位置：放在半空中
    );

    // ==========================================
    // 2. 建立「動態掉落的圓球」 (用來測試滾動與旋轉)
    // ==========================================
    auto circlePart = std::make_shared<GameWorld::Circle>(
        50.0f,  // 直徑 50 像素 (半徑 25)
        glm::vec2(0.0F, 0.0F));
    auto circleComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{circlePart},
        GameWorld::BodyType::DYNAMIC,  // 動態剛體 (會掉落)
        glm::vec2(15.0F, 400.0F)       // 絕對位置：放在方塊正上方再稍微偏右，製造不平衡的撞擊！
    );

    auto capsulePart = std::make_shared<GameWorld::Capsule>(
        30.0f,  // 直徑 30 像素
        glm::vec2(-15.0f, 0.0f),  // 膠囊體左端圓心相對位置
        glm::vec2(15.0f, 0.0f)    // 膠囊體右端圓心相對位置
    );
    auto capsuleComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{capsulePart},
        GameWorld::BodyType::DYNAMIC,  // 動態剛體 (會掉落)
        glm::vec2(-15.0F, 400.0F)       // 絕對位置：放在方塊正上方再稍微偏左，製造不平衡的撞擊！
    );
    // ==========================================
    // 3. 建立 600x600 px 邊界（置中）
    // ==========================================
    auto boundary = std::make_shared<GameWorld::Boundary>(glm::vec2{600.0F, 600.0F});

    // ==========================================
    // 4. 將所有組合件打包，並初始化物理世界
    // ==========================================
    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        boxComp, circleComp, capsuleComp, boundary};

    // 實例化物理世界 (建構子會自動將這些 objects 透過 AttachToWorld 掛載到 Box2D)
    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects);
    data.passCondition = std::make_shared<OneToOneContactPass>(boxPart->Getb2ShapeId(), circlePart->Getb2ShapeId(), TriggerType::TOUCHING, 3);

    // 回傳設定好的關卡資料
    return data;
}

// 自動註冊（靜態物件）
namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_1, LevelData_1);
    }
};

static Register reg;
}  // namespace