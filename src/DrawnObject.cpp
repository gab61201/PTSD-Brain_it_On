#include "DrawnObject.hpp"

#include <memory>

#include "Util/Image.hpp"
#include "Util/TransformUtils.hpp"

#define CIRCLE_IMAGE_PATH "Resources/Images/circle.png"
#define SQUARE_IMAGE_PATH "Resources/Images/square.png"
#define DIAMETER 10.0f

namespace {
Util::Image& CircleImage() {
    static std::unique_ptr<Util::Image> circle =
        std::make_unique<Util::Image>(CIRCLE_IMAGE_PATH);
    return *circle;
}

Util::Image& SquareImage() {
    static std::unique_ptr<Util::Image> square =
        std::make_unique<Util::Image>(SQUARE_IMAGE_PATH);
    return *square;
}
} // namespace

DrawnObject::DrawnObject(glm::vec2 first_point) : m_points({first_point}) {}

void DrawnObject::next_stroke(glm::vec2 point) {
    Util::Transform square_data;
    glm::vec2 p1 = m_points.back();
    glm::vec2 p2 = point;
    square_data.translation = (p1 + p2) / 2.0F;

    float distance = glm::distance(p1, p2);
    square_data.scale = glm::vec2(distance / DIAMETER, 1.0f);

    glm::vec2 direction = p2 - p1;
    square_data.rotation = std::atan2(direction.y, direction.x);

    m_path.push_back(square_data);
    m_points.push_back(point);
}

void DrawnObject::render() {
    Util::Transform circle_data;
    for (const auto &point : m_points){
        circle_data.translation = point;
        CircleImage().Draw(Util::ConvertToUniformBufferData(
            circle_data, glm::vec2(DIAMETER), 0.0f));
    }
    for (const auto &data : m_path){
        SquareImage().Draw(Util::ConvertToUniformBufferData(
            data, glm::vec2(DIAMETER), 0.0f));
    }
}