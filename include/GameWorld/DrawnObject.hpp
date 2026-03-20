#ifndef DRAWN_OBJECT_HPP
#define DRAWN_OBJECT_HPP

#include "GameWorld/CompositeObject.hpp"

namespace GameWorld {

class DrawnObject : public GameWorld::CompositeObject {
   public:
    DrawnObject(glm::vec2 position);
    ~DrawnObject() = default;

    void DrawNextPoint(glm::vec2 position);
    void EndDrawing();

   private:
    std::vector<glm::vec2> m_Points;
};

}  // namespace GameWorld

#endif  // DRAWN_OBJECT_HPP