#include "GameWorld/CompositeObject/DrawnObject.hpp"

#include "Constants.hpp"
#include "GameWorld/Shape/Capsule.hpp"
#include "GameWorld/Shape/Circle.hpp"

namespace GameWorld {

DrawnObject::DrawnObject(glm::vec2 position)
    : CompositeObject(std::vector<std::shared_ptr<Shape>>(), BodyType::STATIC) {
    auto first_point = std::make_shared<Circle>(STROKE_WIDTH, position, ShapeColor::White, false);
    first_point->GetVisual()->SetZIndex(Layer::DrawnObject);
    m_Shapes = {first_point};
    m_Points.push_back(position);
    m_Renderer.AddChild(first_point->GetVisual());
}

void DrawnObject::AttachToWorld(b2WorldId world) {
    CompositeObject::AttachToWorld(world);
    for (auto& shape : m_Shapes) {
        b2Shape_EnableSensorEvents(shape->Getb2ShapeId(), true);
    }
}

void DrawnObject::DrawNextPoint(glm::vec2 position) {
    // 如果新點與上一個點距離太近，則不繪製新點
    auto last_point = m_Points.back();
    auto length = glm::distance(position, last_point);
    if (length < MIN_STROKE_LENGTH) {
        return;
    }

    auto new_stroke = std::make_shared<Capsule>(STROKE_WIDTH, last_point, position, ShapeColor::White, false);
    new_stroke->GetVisual()->SetZIndex(Layer::DrawnObject);
    new_stroke->AttachToBody(m_b2BodyId);
    b2Shape_EnableSensorEvents(new_stroke->Getb2ShapeId(), true);
    m_Shapes.push_back(new_stroke);
    m_Renderer.AddChild(new_stroke->GetVisual());
    m_Points.push_back(position);
}

void DrawnObject::EndDrawing() {
    b2Body_SetType(m_b2BodyId, b2_dynamicBody);
}

}  // namespace GameWorld
