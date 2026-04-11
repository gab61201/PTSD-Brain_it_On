#include "GameWorld/Shape/Rectangle.hpp"

namespace {

float IMAGE_SIZE = 417.0F;
std::string RECTANGLE_IMAGE_PATH = "Resources/Images/BasicShapes/white_square.png";

}  // namespace

namespace GameWorld {

Rectangle::Rectangle(const glm::vec2& size, const glm::vec2& relativePosition, bool isSensor) {
    m_Size = size;
    m_RelativePosition = relativePosition;
    m_IsSensor = isSensor;
}

void Rectangle::AttachToBody(b2BodyId body) {
    if (B2_IS_NON_NULL(m_Shape)) return;
    
    glm::vec2 size = std::get<glm::vec2>(m_Size);
    float halfWidth = PixelsToMeters(size.x / 2.0f);
    float halfHeight = PixelsToMeters(size.y / 2.0f);

    b2Vec2 center = PixelsToMeters(m_RelativePosition);
    b2Polygon rectangleShape = b2MakeOffsetBox(halfWidth, halfHeight, center, b2Rot_identity);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = m_IsSensor;
    m_Shape = b2CreatePolygonShape(body, &shapeDef, &rectangleShape);

    m_Visual->SetDrawable(s_ImageCache.Get(RECTANGLE_IMAGE_PATH));
    m_Visual->m_Transform.scale = size / IMAGE_SIZE;
}

}  // namespace GameWorld
