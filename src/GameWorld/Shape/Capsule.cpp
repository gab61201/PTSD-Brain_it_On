#include "GameWorld/Shape/Capsule.hpp"

#include "Constants.hpp"
#include "GameWorld/CoordinateHelper.hpp"

namespace GameWorld {

Capsule::Capsule(
    float diameter,
    const glm::vec2& pointA,
    const glm::vec2& pointB,
    bool isSensor,
    bool outline)
    : Shape(diameter, (pointA + pointB) * 0.5f, std::atan2(pointB.y - pointA.y, pointB.x - pointA.x), isSensor, outline),
      m_PointA(pointA),
      m_PointB(pointB) {}

void Capsule::AttachToBody(b2BodyId body) {
    if (B2_IS_NON_NULL(m_b2ShapeId)) {
        return;
    }

    float diameter = std::get<float>(m_Size);
    b2Capsule capsuleShape = {
        PixelsToMeters(m_PointA),        // 第一個半圓中心
        PixelsToMeters(m_PointB),        // 第二個半圓中心
        PixelsToMeters(diameter * 0.5f)  // 半徑
    };

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = m_IsSensor;
    m_b2ShapeId = b2CreateCapsuleShape(body, &shapeDef, &capsuleShape);

    const glm::vec2 circleScale{diameter / BASIC_SHAPE_IMAGE_SIZE, diameter / BASIC_SHAPE_IMAGE_SIZE};
    // 處理長度小於等於寬度的情況（顯示為圓形）
    if (glm::distance(m_PointA, m_PointB) <= 1.0f) {
        // 顯示為圓形
        m_Visual->SetDrawable(s_ImageCache.Get(Path::WhiteCircle));
        m_Visual->SetZIndex(Layer::Shape);
        m_Visual->m_Transform.scale = circleScale;
    } else {
        // 中間矩形
        m_Visual = std::make_shared<Util::GameObject>();
        m_Visual->SetDrawable(s_ImageCache.Get(Path::WhiteSquare));
        m_Visual->SetZIndex(Layer::Shape);
        m_Visual->m_Transform.scale = glm::vec2(glm::distance(m_PointA, m_PointB), diameter) / BASIC_SHAPE_IMAGE_SIZE;
        // 左圓
        m_CircleAVisual = std::make_shared<Util::GameObject>();
        m_CircleAVisual->SetDrawable(s_ImageCache.Get(Path::WhiteCircle));
        m_CircleAVisual->m_Transform.scale = circleScale;
        m_CircleAVisual->SetZIndex(Layer::Shape);
        m_Visual->AddChild(m_CircleAVisual);
        // 右圓
        m_CircleBVisual = std::make_shared<Util::GameObject>();
        m_CircleBVisual->SetDrawable(s_ImageCache.Get(Path::WhiteCircle));
        m_CircleBVisual->m_Transform.scale = circleScale;
        m_CircleBVisual->SetZIndex(Layer::Shape);
        m_Visual->AddChild(m_CircleBVisual);
    }
}

void Capsule::Update(glm::vec2 ParentObjectPosition, float ParentObjectRotation) {
    Shape::Update(ParentObjectPosition, ParentObjectRotation);

    if (!m_CircleAVisual || !m_CircleBVisual) return;

    float globalRotation = ParentObjectRotation + m_RelativeRotation;
    float halfLength = glm::distance(m_PointA, m_PointB) * 0.5f;
    glm::vec2 globalDirection(std::cos(globalRotation), std::sin(globalRotation));

    glm::vec2 centerPos = m_Visual->m_Transform.translation;
    m_CircleAVisual->m_Transform.translation = centerPos - (globalDirection * halfLength);
    m_CircleBVisual->m_Transform.translation = centerPos + (globalDirection * halfLength);
}

}  // namespace GameWorld
