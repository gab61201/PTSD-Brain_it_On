#ifndef GAMEWORLD_DRAWN_OBJECT_HPP
#define GAMEWORLD_DRAWN_OBJECT_HPP

#include "GameWorld/CompositeObject/CompositeObject.hpp"

namespace GameWorld {

class DrawnObject : public GameWorld::CompositeObject {
   public:
    DrawnObject(glm::vec2 position);
    ~DrawnObject() = default;

    void DrawNextPoint(glm::vec2 position);

    void EndDrawing();

    const std::vector<glm::vec2>& GetPoints() const { return m_Points; }

   private:
    std::vector<glm::vec2> m_Points;
};

}  // namespace GameWorld

#endif  // GAMEWORLD_DRAWN_OBJECT_HPP