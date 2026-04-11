#include "GameWorld/Shape/Circle.hpp"

namespace {

float IMAGE_SIZE = 417.0F;
std::string CIRCLE_IMAGE_PATH = "Resources/Images/BasicShapes/white_circle.png";

}  // namespace

namespace GameWorld {

Circle::Circle(float diameter, const glm::vec2& relativePosition, bool isSensor) {
    m_Size = diameter;
    m_RelativePosition = relativePosition;
    m_IsSensor = isSensor;
}

void Circle::AttachToBody(b2BodyId body) {
    if (B2_IS_NON_NULL(m_Shape)) return;
    b2Circle circleShape = {
        PixelsToMeters(m_RelativePosition),             // center
        PixelsToMeters(std::get<float>(m_Size) / 2.0f)  // radius
    };
    b2ShapeId m_Shape = b2CreateCircleShape(body, &m_ShapeDef, &circleShape);

    m_Visual->SetDrawable(s_ImageCache.Get(CIRCLE_IMAGE_PATH));
    m_Visual->m_Transform.scale = glm::vec2(std::get<float>(m_Size), std::get<float>(m_Size)) / IMAGE_SIZE;
}

}  // namespace GameWorld