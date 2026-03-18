#include "GameWorld/DrawnObject.hpp"

#define MIN_STROKE_LENGTH 2.0F
#define STROKE_WIDTH 10.0F


namespace GameWorld {

DrawnObject::DrawnObject(glm::vec2 position) {
    m_BodyType = BodyType::STATIC;
    m_Position = glm::vec2(0.0F, 0.0F);
    m_Rotation = 0.0F;
    auto first_point = std::make_shared<BaseObject>(ShapeType::CIRCLE, glm::vec2(10.0F, 10.0F), position);
    m_BaseObjects = {first_point};
    m_Points.push_back(position);
}

void DrawnObject::DrawNextPoint(glm::vec2 position) {
    auto last_point = m_Points.back();
    auto length = glm::distance(position, last_point);
    if (length < MIN_STROKE_LENGTH) {
        return;
    }

    auto new_point = std::make_shared<BaseObject>(
        ShapeType::CIRCLE,
        glm::vec2(STROKE_WIDTH, STROKE_WIDTH),
        position);
    new_point->AttachToBody(m_Body);
    m_BaseObjects.push_back(new_point);

    float dx = position.x - last_point.x;
    float dy = position.y - last_point.y;
    float angle = std::atan2(dy, dx);
    auto new_stroke = std::make_shared<BaseObject>(
        ShapeType::RECTANGLE,
        glm::vec2(length, STROKE_WIDTH),
        (position + m_Points.back()) / 2.0F,
        angle);
    new_stroke->AttachToBody(m_Body);
    m_BaseObjects.push_back(new_stroke);

    m_Points.push_back(position);
}

void DrawnObject::EndDrawing() {
    m_Body->SetType(b2_dynamicBody);
}

}  // namespace GameWorld
