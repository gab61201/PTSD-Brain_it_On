#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/PassCondition.hpp"

class GlassFallPass : public PassCondition {
   public:
    GlassFallPass(b2ShapeId leftWall, b2ShapeId rightWall, b2ShapeId ground, int duration)
        : PassCondition(TriggerType::TOUCHING, duration),
          m_LeftWall(leftWall),
          m_RightWall(rightWall),
          m_Ground(ground) {
        b2Shape_EnableContactEvents(leftWall, true);
        b2Shape_EnableContactEvents(rightWall, true);
        b2Shape_EnableContactEvents(ground, true);
    }

   protected:
    void OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) override {
        bool involvesWall = B2_ID_EQUALS(shapeA, m_LeftWall) || B2_ID_EQUALS(shapeB, m_LeftWall) ||
                            B2_ID_EQUALS(shapeA, m_RightWall) || B2_ID_EQUALS(shapeB, m_RightWall);
        bool involvesGround = B2_ID_EQUALS(shapeA, m_Ground) || B2_ID_EQUALS(shapeB, m_Ground);
        if (involvesWall && involvesGround) {
            if (triggerType == TriggerType::TOUCHING) {
                m_ContactCount++;
            } else if (triggerType == TriggerType::SEPARATED) {
                m_ContactCount--;
            }
        }
        m_IsTriggered = (m_ContactCount > 0);
    }

   private:
    b2ShapeId m_LeftWall;
    b2ShapeId m_RightWall;
    b2ShapeId m_Ground;
    int m_ContactCount = 0;
};

LevelConfig LevelConfig_9() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Tip the glass onto the ground";

    auto leftWall = std::make_shared<GameWorld::Rectangle>(glm::vec2(15.0F, 120.0F), glm::vec2(-50.0F, 0.0F));
    auto rightWall = std::make_shared<GameWorld::Rectangle>(glm::vec2(15.0F, 120.0F), glm::vec2(50.0F, 0.0F));
    auto bottom = std::make_shared<GameWorld::Rectangle>(glm::vec2(115.0F, 15.0F), glm::vec2(0.0F, -60.0F));

    auto glassComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{leftWall, rightWall, bottom},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, -220.0F));

    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {boundary, glassComp};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);
    data.passCondition = std::make_shared<GlassFallPass>(
        leftWall->Getb2ShapeId(),
        rightWall->Getb2ShapeId(),
        boundary->GetBottomWall().Getb2ShapeId(),
        3);

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_9, LevelConfig_9);
    }
};
static Register reg;
}  // namespace
