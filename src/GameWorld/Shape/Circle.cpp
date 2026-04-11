#include "GameWorld/Shape/Circle.hpp"

namespace {

float IMAGE_SIZE = 417.0F;
std::string CIRCLE_IMAGE_PATH = "Resources/Images/BasicShapes/white_circle.png";

}  // namespace

namespace GameWorld {

Circle::Circle(float diameter, const glm::vec2& relativePosition, bool isSensor)
: Shape(diameter, relativePosition, 0.0f, isSensor) {}

void Circle::AttachToBody(b2BodyId body) {
    if (B2_IS_NON_NULL(m_b2ShapeId)) return;

    b2Circle circleShape = {
        PixelsToMeters(m_RelativePosition),             // center
        PixelsToMeters(std::get<float>(m_Size) / 2.0f)  // radius
    };
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = m_IsSensor;
    m_b2ShapeId = b2CreateCircleShape(body, &shapeDef, &circleShape);

    m_Visual->SetDrawable(s_ImageCache.Get(CIRCLE_IMAGE_PATH));
    m_Visual->m_Transform.scale = glm::vec2(std::get<float>(m_Size), std::get<float>(m_Size)) / IMAGE_SIZE;
}

}  // namespace GameWorld