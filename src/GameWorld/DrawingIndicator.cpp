#include "GameWorld/DrawingIndicator.hpp"

#include "Constants.hpp"

#include "Util/ImageCache.hpp"

namespace GameWorld {

DrawingIndicator::DrawingIndicator() {

    const float circleScale = STROKE_WIDTH / BASIC_SHAPE_IMAGE_SIZE;

    m_P1 = std::make_shared<Util::GameObject>(Util::ImageCache.Get(Path::RedCircleTrans), Layer::UIOutline, glm::vec2(0.0f, 0.0f), false);
    m_P1->m_Transform.scale = {circleScale, circleScale};
    m_Renderer.AddChild(m_P1);

    m_P2 = std::make_shared<Util::GameObject>(Util::ImageCache.Get(Path::RedCircleTrans), Layer::UIOutline, glm::vec2(0.0f, 0.0f), false);
    m_P2->m_Transform.scale = {circleScale, circleScale};
    m_Renderer.AddChild(m_P2);

    m_Line = std::make_shared<Util::GameObject>(Util::ImageCache.Get(Path::RedLine), Layer::UIOutline, glm::vec2(0.0f, 0.0f), false);
    m_Line->m_Transform.scale = {1.0f, STROKE_WIDTH};
    m_Renderer.AddChild(m_Line);
}

void DrawingIndicator::DrawLine(const glm::vec2& p1, const glm::vec2& p2) {
    m_P1->m_Transform.translation = p1;
    m_P2->m_Transform.translation = p2;

    m_Line->m_Transform.translation = (p1 + p2) * 0.5f;
    m_Line->m_Transform.rotation = atan2(p2.y - p1.y, p2.x - p1.x);
    m_Line->m_Transform.scale.x = glm::distance(p1, p2) / BASIC_SHAPE_IMAGE_SIZE;

    m_P1->SetVisible(true);
    m_P2->SetVisible(true);
    m_Line->SetVisible(true);
}

void DrawingIndicator::Update() {
    m_Renderer.Update();
    m_P1->SetVisible(false);
    m_P2->SetVisible(false);
    m_Line->SetVisible(false);
}

}  // namespace GameWorld
