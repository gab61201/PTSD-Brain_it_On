#include "GameWorld/CompositeObject/Boundary.hpp"

#include <glm/glm.hpp>

namespace GameWorld {

Boundary::Boundary(glm::vec2 size, glm::vec2 center, bool buildTop, bool buildBottom, bool buildLeft, bool buildRight) {
    constexpr float THICKNESS = 10.0F;

    std::vector<std::shared_ptr<Shape>> shapes;

    // Left wall (vertical) — extends beyond top/bottom walls by THICKNESS/2 on each side
    if (buildLeft) {
        glm::vec2 pos = {- size.x * 0.5F - THICKNESS * 0.5F + center.x, center.y};
        float height = size.y;

        shapes.push_back(std::make_shared<Rectangle>(
            glm::vec2(THICKNESS, height), pos));
    }

    // Right wall (vertical) — extends beyond top/bottom walls by THICKNESS/2 on each side
    if (buildRight) {
        glm::vec2 pos = {size.x * 0.5F + THICKNESS * 0.5F + center.x, center.y};
        float height = size.y;

        shapes.push_back(std::make_shared<Rectangle>(
            glm::vec2(THICKNESS, height), pos));
    }

    // Top wall (horizontal) — fits between left and right walls
    if (buildTop) {
        glm::vec2 pos = {center.x, size.y * 0.5F + THICKNESS * 0.5F + center.y};
        float width = size.x + THICKNESS * 2.0F;

        shapes.push_back(std::make_shared<Rectangle>(
            glm::vec2(width, THICKNESS), pos));
    }

    // Bottom wall (horizontal) — fits between left and right walls
    if (buildBottom) {
        glm::vec2 pos = {center.x, - size.y * 0.5F - THICKNESS * 0.5F + center.y};
        float width = size.x + THICKNESS * 2.0F;

        shapes.push_back(std::make_shared<Rectangle>(
            glm::vec2(width, THICKNESS), pos));
    }

    m_Shapes = std::move(shapes);
    m_BodyType = BodyType::STATIC;
}

}  // namespace GameWorld
