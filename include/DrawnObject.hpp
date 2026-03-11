#ifndef DRAWN_OBJECT_HPP
#define DRAWN_OBJECT_HPP
#include "PhysicalObject.hpp"
#include "glm/vec2.hpp"

class DrawnObject {
   public:
    DrawnObject(glm::vec2);
    ~DrawnObject() = default;

    void render();
    void next_stroke(glm::vec2);
   private:
    std::vector<glm::vec2> m_points;        // for circles
    std::vector<Util::Transform> m_path;    // for squares
};

#endif
