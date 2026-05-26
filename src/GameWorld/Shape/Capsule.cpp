#include "GameWorld/Shape/Capsule.hpp"

#include "Constants.hpp"
#include "Util/ImageCache.hpp"
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
    const float distance = glm::distance(m_PointA, m_PointB);

    if (distance <= 1.0f) {
        // --- 情況 A：長度極短，顯示為純圓形 ---
        m_Visual->SetDrawable(Util::ImageCache.Get(Path::WhiteCircle));
        m_Visual->m_Transform.scale = circleScale;

        if (m_OutlineVisual) {
            m_OutlineVisual->SetDrawable(Util::ImageCache.Get(Path::BlackCircle));
            m_OutlineVisual->m_Transform.scale = 
                glm::vec2(diameter + SHAPE_OUTLINE_WIDTH, diameter + SHAPE_OUTLINE_WIDTH) / BASIC_SHAPE_IMAGE_SIZE;
        }
    } else {
        // --- 情況 B：正常膠囊形 ---
        // 1. 中間矩形主體（直接利用已存在的 m_Visual）
        m_Visual->SetDrawable(Util::ImageCache.Get(Path::WhiteSquare));
        m_Visual->m_Transform.scale = glm::vec2(distance, diameter) / BASIC_SHAPE_IMAGE_SIZE;

        // 2. 中間矩形描邊（直接利用已存在的 m_OutlineVisual）
        if (m_OutlineVisual) {
            m_OutlineVisual->SetDrawable(Util::ImageCache.Get(Path::BlackSquare));
            m_OutlineVisual->m_Transform.scale = 
                glm::vec2(distance, diameter + SHAPE_OUTLINE_WIDTH) / BASIC_SHAPE_IMAGE_SIZE;
        }

        // 3. 初始化兩端圓形主體 (白色)
        m_CircleAVisual = std::make_shared<Util::GameObject>(Util::ImageCache.Get(Path::WhiteCircle), Layer::Shape);
        m_CircleAVisual->m_Transform.scale = circleScale;
        m_Visual->AddChild(m_CircleAVisual);

        m_CircleBVisual = std::make_shared<Util::GameObject>(Util::ImageCache.Get(Path::WhiteCircle), Layer::Shape);
        m_CircleBVisual->m_Transform.scale = circleScale;
        m_Visual->AddChild(m_CircleBVisual);

        // 4. 初始化兩端圓形描邊 (黑色)
        if (m_OutlineVisual) {
            const glm::vec2 outlineCircleScale = 
                glm::vec2(diameter + SHAPE_OUTLINE_WIDTH, diameter + SHAPE_OUTLINE_WIDTH) / BASIC_SHAPE_IMAGE_SIZE;

            m_CircleAOutlineVisual = std::make_shared<Util::GameObject>(Util::ImageCache.Get(Path::BlackCircle), Layer::ShapeOutLine);
            m_CircleAOutlineVisual->m_Transform.scale = outlineCircleScale;
            m_OutlineVisual->AddChild(m_CircleAOutlineVisual);

            m_CircleBOutlineVisual = std::make_shared<Util::GameObject>(Util::ImageCache.Get(Path::BlackCircle), Layer::ShapeOutLine);
            m_CircleBOutlineVisual->m_Transform.scale = outlineCircleScale;
            m_OutlineVisual->AddChild(m_CircleBOutlineVisual);
        }
    }
}

void Capsule::Update(glm::vec2 ParentObjectPosition, float ParentObjectRotation) {
    Shape::Update(ParentObjectPosition, ParentObjectRotation);

    if (!m_CircleAVisual || !m_CircleBVisual) return;

    float globalRotation = ParentObjectRotation + m_RelativeRotation;
    float halfLength = glm::distance(m_PointA, m_PointB) * 0.5f;
    glm::vec2 globalDirection(std::cos(globalRotation), std::sin(globalRotation));

    glm::vec2 centerPos = m_Visual->m_Transform.translation;
    glm::vec2 offset = globalDirection * halfLength;

    m_CircleAVisual->m_Transform.translation = centerPos - offset;
    m_CircleAVisual->m_Transform.rotation = globalRotation;

    m_CircleBVisual->m_Transform.translation = centerPos + offset;
    m_CircleBVisual->m_Transform.rotation = globalRotation;

    if (m_CircleAOutlineVisual && m_CircleBOutlineVisual) {
        m_CircleAOutlineVisual->m_Transform.translation = centerPos - offset;
        m_CircleAOutlineVisual->m_Transform.rotation = globalRotation;

        m_CircleBOutlineVisual->m_Transform.translation = centerPos + offset;
        m_CircleBOutlineVisual->m_Transform.rotation = globalRotation;
    }
}

}  // namespace GameWorld
