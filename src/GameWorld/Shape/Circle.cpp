#include "GameWorld/Shape/Circle.hpp"

#include "Constants.hpp"
#include "Util/ImageCache.hpp"
#include "GameWorld/CoordinateHelper.hpp"

namespace GameWorld {

Circle::Circle(float diameter, const glm::vec2& relativePosition, bool isSensor, bool outline, bool isForbidden)
    : Shape(diameter, relativePosition, 0.0f, isSensor, outline, isForbidden) {}

void Circle::AttachToBody(b2BodyId body) {
    if (B2_IS_NON_NULL(m_b2ShapeId)) {
        return;
    }

    b2Circle circleShape = {
        PixelsToMeters(m_RelativePosition),             // center
        PixelsToMeters(std::get<float>(m_Size) / 2.0f)  // radius
    };
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = m_IsSensor;
    m_b2ShapeId = b2CreateCircleShape(body, &shapeDef, &circleShape);

    if (m_IsForbidden) {
        m_Visual->SetDrawable(Util::ImageCache.Get(Path::RedCircleTrans));
    } else {
        m_Visual->SetDrawable(Util::ImageCache.Get(Path::WhiteCircle));
    }
    m_Visual->m_Transform.scale = glm::vec2(std::get<float>(m_Size), std::get<float>(m_Size)) / BASIC_SHAPE_IMAGE_SIZE;

    if (m_OutlineVisual) {
        m_OutlineVisual->SetDrawable(Util::ImageCache.Get(Path::BlackCircle));
        m_OutlineVisual->m_Transform.scale =
            glm::vec2(std::get<float>(m_Size) + SHAPE_OUTLINE_WIDTH,
                      std::get<float>(m_Size) + SHAPE_OUTLINE_WIDTH) /
            BASIC_SHAPE_IMAGE_SIZE;
    }
}

}  // namespace GameWorld