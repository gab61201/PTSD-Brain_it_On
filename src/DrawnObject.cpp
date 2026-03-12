#include "DrawnObject.hpp"

#include <memory>

#include "Util/Image.hpp"
#include "Util/TransformUtils.hpp"

#define CIRCLE_IMAGE_PATH "Resources/Images/circle.png"
#define SQUARE_IMAGE_PATH "Resources/Images/square.png"
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
        std::make_unique<Util::Image>(SQUARE_IMAGE_PATH);
    return *square;
}
}  // namespace

DrawnObject::DrawnObject(glm::vec2 first_point) : m_Points({first_point}) {}

// 更新筆畫的最前端
void DrawnObject::NextStroke(glm::vec2 point) {
    Util::Transform square_data;
    glm::vec2 p1 = m_Points.back();
    glm::vec2 p2 = point;
    square_data.translation = (p1 + p2) / 2.0F;

    float distance = glm::distance(p1, p2);
    square_data.scale = glm::vec2(distance / STROKE_WIDTH, 1.0f);

    glm::vec2 direction = p2 - p1;
    square_data.rotation = std::atan2(direction.y, direction.x);

    m_Path.push_back(square_data);
    m_Points.push_back(point);
}

// 渲染整個物件
void DrawnObject::Render() {
    Util::Transform circle_data;
    for (const auto& point : m_Points) {
        circle_data.translation = point;
        CircleImage().Draw(Util::ConvertToUniformBufferData(
            circle_data, glm::vec2(STROKE_WIDTH), 0.0f));
    }
    for (const auto& data : m_Path) {
        SquareImage().Draw(Util::ConvertToUniformBufferData(
            data, glm::vec2(STROKE_WIDTH), 0.0f));
    }
}