#include "GameWorld/CompositeObject/DrawnObject.hpp"

#include "GameWorld/Shape/Capsule.hpp"
#include "GameWorld/Shape/Circle.hpp"

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

    auto new_stroke = std::make_shared<Capsule>(STROKE_WIDTH, last_point, position, false);
    new_stroke->AttachToBody(m_b2BodyId);
    m_Shapes.push_back(new_stroke);
    m_Renderer.AddChild(new_stroke->GetVisual());
    m_Points.push_back(position);
}

void DrawnObject::EndDrawing() {
    b2Body_SetType(m_b2BodyId, b2_dynamicBody);
}

}  // namespace GameWorld
