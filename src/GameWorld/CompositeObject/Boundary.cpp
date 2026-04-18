#include "GameWorld/CompositeObject/Boundary.hpp"

#include <glm/glm.hpp>

namespace {

float IMAGE_SIZE = 886.0F;
std::string LEVEL_IMAGE_PATH = "Resources/Images/level_background.png";

}

namespace GameWorld {

Boundary::Boundary(float x1, float x2, float y1, float y2) {
    constexpr float THICKNESS = 10.0F;

    float centerX = (x1 + x2) * 0.5F;
    float centerY = (y1 + y2) * 0.5F;
    float width = x2 - x1;
    float height = y2 - y1;

    std::vector<std::shared_ptr<Shape>> shapes;

    // Left wall
    shapes.push_back(std::make_shared<Rectangle>(
        glm::vec2(THICKNESS, height), glm::vec2(x1 + THICKNESS * 0.5F, centerY)));

    // Right wall
    shapes.push_back(std::make_shared<Rectangle>(
        glm::vec2(THICKNESS, height), glm::vec2(x2 - THICKNESS * 0.5F, centerY)));

    // Top wall
    shapes.push_back(std::make_shared<Rectangle>(
        glm::vec2(width - THICKNESS * 2.0F, THICKNESS), glm::vec2(centerX, y2 - THICKNESS * 0.5F)));

    // Bottom wall
    shapes.push_back(std::make_shared<Rectangle>(
        glm::vec2(width - THICKNESS * 2.0F, THICKNESS), glm::vec2(centerX, y1 + THICKNESS * 0.5F)));

    m_Shapes = std::move(shapes);
    m_BodyType = BodyType::STATIC;
    m_X1 = x1; m_X2 = x2; m_Y1 = y1; m_Y2 = y2;

    auto levelImage = std::make_shared<Util::Image>(LEVEL_IMAGE_PATH);
    auto boundaryVisual = std::make_shared<Util::GameObject>(levelImage, -0.1F);
    boundaryVisual->m_Transform.translation = {centerX, centerY};
    boundaryVisual->m_Transform.scale = {width / IMAGE_SIZE, height / IMAGE_SIZE};
    m_Renderer.AddChild(boundaryVisual);
}

bool Boundary::IsPointInside(glm::vec2 position) const {
    return (position.x > m_X1 && position.x < m_X2 &&
            position.y > m_Y1 && position.y < m_Y2);
}

}  // namespace GameWorld
