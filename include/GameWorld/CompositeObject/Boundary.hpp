#pragma once

#include <glm/glm.hpp>

#include "GameWorld/CompositeObject/CompositeObject.hpp"

namespace GameWorld {

class Boundary : public CompositeObject {
   public:
    Boundary(float x1, float x2, float y1, float y2);
    ~Boundary() = default;

    bool IsPointInside(glm::vec2 position) const {
        return (position.x > m_X1 && position.x < m_X2 &&
                position.y > m_Y1 && position.y < m_Y2);
    }

    const Shape& GetLeftWall() const { return *m_Shapes[0]; }
    const Shape& GetRightWall() const { return *m_Shapes[1]; }
    const Shape& GetTopWall() const { return *m_Shapes[2]; }
    const Shape& GetBottomWall() const { return *m_Shapes[3]; }

   private:
    float m_X1 = 0.0F;
    float m_X2 = 0.0F;
    float m_Y1 = 0.0F;
    float m_Y2 = 0.0F;
};

}  // namespace GameWorld
