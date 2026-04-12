#include "GameWorld/DrawnObject.hpp"

#include "GameWorld/Shape/Circle.hpp"
#include "GameWorld/Shape/Rectangle.hpp"

#define MIN_STROKE_LENGTH 2.0F
#define STROKE_WIDTH 10.0F

namespace GameWorld {

DrawnObject::DrawnObject(glm::vec2 position)
: CompositeObject(std::vector<std::shared_ptr<Shape>>()) {
    auto first_point = std::make_shared<Circle>(STROKE_WIDTH, position, false);
    m_Shapes = {first_point};
    m_Points.push_back(position);
    m_Renderer.AddChild(first_point->GetVisual());
}

void DrawnObject::DrawNextPoint(glm::vec2 position) {
    // 如果新點與上一個點距離太近，則不繪製新點
    auto last_point = m_Points.back();
    auto length = glm::distance(position, last_point);
    if (length < MIN_STROKE_LENGTH) {
        return;
    }

    // 建立一個圓形來表示新點
    auto new_point = std::make_shared<Circle>(STROKE_WIDTH, position, false);
    new_point->AttachToBody(m_b2BodyId);
    m_Shapes.push_back(new_point);
    m_Renderer.AddChild(new_point->GetVisual());

    // 建立一個矩形來連接上一個點和當前點
    float dx = position.x - last_point.x;
    float dy = position.y - last_point.y;
    float angle = std::atan2(dy, dx);
    auto new_stroke = std::make_shared<Rectangle>(
        glm::vec2(length, STROKE_WIDTH),
        (position + m_Points.back()) * 0.5F,
        angle);
    new_stroke->AttachToBody(m_b2BodyId);
    m_Shapes.push_back(new_stroke);
    m_Renderer.AddChild(new_stroke->GetVisual());

    // 更新點列表
    m_Points.push_back(position);
}

void DrawnObject::EndDrawing() {
    b2Body_SetType(m_b2BodyId, b2_dynamicBody);
}

}  // namespace GameWorld
