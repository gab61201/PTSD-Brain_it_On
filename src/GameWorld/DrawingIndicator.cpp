#include "GameWorld/DrawingIndicator.hpp"

namespace {

float STROKE_WIDTH = 10.0F;
float IMAGE_SIZE = 417.0F;
float SCALE = STROKE_WIDTH / IMAGE_SIZE;
std::string CIRCLE_IMAGE_PATH = "Resources/Images/BasicShapes/white_circle.png";
std::string RECTANGLE_IMAGE_PATH = "Resources/Images/BasicShapes/white_square.png";

}  // namespace

namespace GameWorld {

Util::AssetStore<std::shared_ptr<Util::Image>> DrawingIndicator::s_ImageCache{
    [](const std::string& filepath) {
        return std::make_shared<Util::Image>(filepath);
    }};

DrawingIndicator::DrawingIndicator() {
    m_P1 = std::make_shared<Util::GameObject>(s_ImageCache.Get(CIRCLE_IMAGE_PATH), 3.0f, glm::vec2(0.0f, 0.0f), false);
    m_P1->m_Transform.scale = {SCALE, SCALE};
    m_Renderer.AddChild(m_P1);
    
    m_P2 = std::make_shared<Util::GameObject>(s_ImageCache.Get(CIRCLE_IMAGE_PATH), 3.0f, glm::vec2(0.0f, 0.0f), false);
    m_P2->m_Transform.scale = {SCALE, SCALE};
    m_Renderer.AddChild(m_P2);
    
    m_Line = std::make_shared<Util::GameObject>(s_ImageCache.Get(RECTANGLE_IMAGE_PATH), 3.0f, glm::vec2(0.0f, 0.0f), false);
    m_Line->m_Transform.scale = {1.0f, SCALE};
    m_Renderer.AddChild(m_Line);
}

void DrawingIndicator::DrawLine(const glm::vec2& p1, const glm::vec2& p2) {
    m_P1->m_Transform.translation = p1;
    m_P2->m_Transform.translation= p2;

    glm::vec2 delta = p2 - p1;
    float length = glm::length(delta);
    float angle = atan2(delta.y, delta.x);

    m_Line->m_Transform.translation = (p1 + p2) * 0.5f;
    m_Line->m_Transform.rotation = angle;
    m_Line->m_Transform.scale.x = length / IMAGE_SIZE;

    m_P1->SetVisible(true);
    m_P2->SetVisible(true);
    m_Line->SetVisible(true);
}

void DrawingIndicator::Update(){
    m_Renderer.Update();
    m_P1->SetVisible(false);
    m_P2->SetVisible(false);
    m_Line->SetVisible(false);
}

} // namespace GameWorld
