#ifndef DRAWN_OBJECT_HPP
#define DRAWN_OBJECT_HPP
#include "PhysicalObject.hpp"
#include "glm/vec2.hpp"

class DrawnObject {
   public:
    DrawnObject(glm::vec2);
    ~DrawnObject() = default;

    void Render();
    void NextStroke(glm::vec2);

   private:
    // 圓的座標
    std::vector<glm::vec2> m_CirclePosition;

    // 方形的位置、座標、比例
    std::vector<Util::Transform> m_RectangleTransform;
};

#endif
