#include "GameWorld/Shape/Rectangle.hpp"

#include "Constants.hpp"
#include "Util/ImageCache.hpp"
#include "GameWorld/CoordinateHelper.hpp"

namespace GameWorld {

Rectangle::Rectangle(const glm::vec2& size, const glm::vec2& relativePosition, float relativeRotation, ShapeColor color, bool isSensor, bool outline, bool isForbidden)
    : Shape(size, relativePosition, relativeRotation, color, isSensor, outline, isForbidden) {}

void Rectangle::AttachToBody(b2BodyId body) {
    if (B2_IS_NON_NULL(m_b2ShapeId)) {
        return;
    }


    glm::vec2 size = std::get<glm::vec2>(m_Size);
    float halfWidth = PixelsToMeters(size.x / 2.0f);
    float halfHeight = PixelsToMeters(size.y / 2.0f);

    b2Vec2 center = PixelsToMeters(m_RelativePosition);
    b2Polygon rectangleShape = b2MakeOffsetBox(halfWidth, halfHeight, center, b2MakeRot(m_RelativeRotation));

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = m_IsSensor;
    m_b2ShapeId = b2CreatePolygonShape(body, &shapeDef, &rectangleShape);

    if (m_IsForbidden) {
        m_Visual->SetDrawable(Util::ImageCache.Get(Path::RedSquareTrans));
    } else {
        switch (m_Color) {
            case ShapeColor::Orange:
                m_Visual->SetDrawable(Util::ImageCache.Get(Path::OrangeSquare));
                break;
            case ShapeColor::Red:
                m_Visual->SetDrawable(Util::ImageCache.Get(Path::RedSquare));
                break;
            default:
                m_Visual->SetDrawable(Util::ImageCache.Get(Path::WhiteSquare));
                break;
        }
    }
    m_Visual->m_Transform.scale = glm::vec2(size.x, size.y) / BASIC_SHAPE_IMAGE_SIZE;

    if (m_OutlineVisual) {
        m_OutlineVisual->SetDrawable(Util::ImageCache.Get(Path::BlackSquare));
        m_OutlineVisual->m_Transform.scale =
            glm::vec2(size.x + SHAPE_OUTLINE_WIDTH, size.y + SHAPE_OUTLINE_WIDTH) /
            BASIC_SHAPE_IMAGE_SIZE;
    }
}

}  // namespace GameWorld
