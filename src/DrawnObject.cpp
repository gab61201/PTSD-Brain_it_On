#include "DrawnObject.hpp"

#include <memory>

#include "Util/Image.hpp"
#include "Util/TransformUtils.hpp"

#define CIRCLE_IMAGE_PATH "Resources/Images/circle.png"
#define RECTANGLE_IMAGE_PATH "Resources/Images/square.png"
#define STROKE_WIDTH 10.0f

namespace {

// circle PNG to Util::Image
Util::Image& CircleImage() {
    static std::unique_ptr<Util::Image> circle =
        std::make_unique<Util::Image>(CIRCLE_IMAGE_PATH);
    return *circle;
}

// square PNG to Util::Image
Util::Image& SquareImage() {
    static std::unique_ptr<Util::Image> square =
        std::make_unique<Util::Image>(RECTANGLE_IMAGE_PATH);
    return *square;
}
}  // namespace

DrawnObject::DrawnObject(glm::vec2 first_point) : m_CirclePosition({first_point}) {}

// 更新筆畫的最前端
void DrawnObject::NextStroke(glm::vec2 point) {
    Util::Transform rectangle_transform;
    glm::vec2 p1 = m_CirclePosition.back();
    glm::vec2 p2 = point;
    rectangle_transform.translation = (p1 + p2) / 2.0F;

    float distance = glm::distance(p1, p2);
    rectangle_transform.scale = glm::vec2(distance / STROKE_WIDTH, 1.0f);

    glm::vec2 direction = p2 - p1;
    rectangle_transform.rotation = std::atan2(direction.y, direction.x);

    m_RectangleTransform.push_back(rectangle_transform);
    m_CirclePosition.push_back(point);
}

// 渲染整個物件
void DrawnObject::Render() {
    Util::Transform circle_transform;
    for (const auto& point : m_CirclePosition) {
        circle_transform.translation = point;
        CircleImage().Draw(Util::ConvertToUniformBufferData(
            circle_transform, glm::vec2(STROKE_WIDTH), 0.0f));
    }
    for (const auto& data : m_RectangleTransform) {
        SquareImage().Draw(Util::ConvertToUniformBufferData(
            data, glm::vec2(STROKE_WIDTH), 0.0f));
    }
}