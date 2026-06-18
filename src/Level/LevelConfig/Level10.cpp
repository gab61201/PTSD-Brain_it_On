#include "GameWorld/CompositeObject/Boundary.hpp"
#include "GameWorld/Shape/Rectangle.hpp"
#include "Level/LevelData.hpp"
#include "Level/PassCondition/ShapeToShapeContactPass.hpp"
#include <vector>

LevelConfig LevelConfig_10() {
    LevelConfig data;
    data.timeout = 10.0F;
    data.strokeLimit = 1;
    data.targetText = "Remove the cap";

    // --- 1. Static triangle ---
    // Built from a narrow "neck" column at the apex + stacked horizontal slices.
    // The neck is narrower and taller than the old flat top, giving the cap
    // room to wrap around and grip.
    //
    // CompositeObject at (0, -80):
    //   Neck: 30×22 at relative (0, 11) → world y [-80, -58]
    //   Slices: from relative y=0 downward, widening from 30px to 320px

    const float neckWidth = 30.0F;
    const float neckHeight = 22.0F;
    const float baseWidth = 320.0F;
    const int numSlices = 10;
    const float sliceSpacing = 21.0F;
    const float sliceHeight = sliceSpacing + 4.0F;  // 25px, slight overlap

    // Neck — tall narrow column for the cap to grip
    auto neck = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(neckWidth, neckHeight),
        glm::vec2(0.0F, neckHeight / 2.0F),  // relative (0, 11)
        0.0F,
        GameWorld::ShapeColor::White,
        false,   // not a sensor
        false);  // no outline for seamless fill

    std::vector<std::shared_ptr<GameWorld::Shape>> triangleShapes = {neck};

    // Stacked slices forming the triangle body below the neck
    const float totalSliceDepth =
        static_cast<float>(numSlices) * sliceSpacing;  // 210
    for (int i = 0; i < numSlices; i++) {
        float fi = static_cast<float>(i);
        float yCenter = -(fi + 0.5F) * sliceSpacing;
        float depth = (fi + 0.5F) * sliceSpacing;
        float width =
            neckWidth + (baseWidth - neckWidth) * depth / totalSliceDepth;

        triangleShapes.push_back(std::make_shared<GameWorld::Rectangle>(
            glm::vec2(width, sliceHeight),
            glm::vec2(0.0F, yCenter),
            0.0F,
            GameWorld::ShapeColor::White,
            false,   // not a sensor
            false));  // no outline for seamless fill
    }

    auto triangleComp = std::make_shared<GameWorld::CompositeObject>(
        triangleShapes,
        GameWorld::BodyType::STATIC,
        glm::vec2(0.0F, -80.0F));

    // --- 2. Dynamic orange cap (U-bracket wrapping the neck) ---
    // The cap is shaped like an inverted U:
    //   - Top bar rests on the neck top surface
    //   - Two side walls extend downward, gripping the neck
    //
    // Cap CompositeObject at (0, -52):
    //   Bar bottom at world -58 = neck top at world -58
    //   Walls extend 17px below bar, wrapping the neck down to world -75
    //   Inner gap: 38px (4px clearance per side on 30px neck)

    auto capBar = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(70.0F, 12.0F),
        glm::vec2(0.0F, 0.0F),
        0.0F,
        GameWorld::ShapeColor::Orange);

    auto capLeftWall = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(16.0F, 17.0F),
        glm::vec2(-27.0F, -14.5F),
        0.0F,
        GameWorld::ShapeColor::Orange);

    auto capRightWall = std::make_shared<GameWorld::Rectangle>(
        glm::vec2(16.0F, 17.0F),
        glm::vec2(27.0F, -14.5F),
        0.0F,
        GameWorld::ShapeColor::Orange);

    // Bar bottom = -52 - 6 = -58 = neck top (world y)
    auto capComp = std::make_shared<GameWorld::CompositeObject>(
        std::vector<std::shared_ptr<GameWorld::Shape>>{
            capBar, capLeftWall, capRightWall},
        GameWorld::BodyType::DYNAMIC,
        glm::vec2(0.0F, -52.0F));

    // --- 3. Boundary ---
    auto boundary = std::make_shared<GameWorld::Boundary>();

    std::vector<std::shared_ptr<GameWorld::CompositeObject>> objects = {
        boundary, triangleComp, capComp};

    data.world = std::make_shared<GameWorld::PhysicalWorld>(objects, boundary);

    // --- 4. Pass condition: cap bar separates from the neck ---
    data.passConditions = {
        std::make_shared<ShapeToShapeContactPass>(
            capBar->Getb2ShapeId(),
            neck->Getb2ShapeId(),
            TriggerType::SEPARATED, 3)
    };

    return data;
}

namespace {
struct Register {
    Register() {
        RegisterLevel(LevelId::LEVEL_10, LevelConfig_10);
    }
};
static Register reg;
}  // namespace
