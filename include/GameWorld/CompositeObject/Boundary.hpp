#pragma once

#include <glm/glm.hpp>

#include "GameWorld/CompositeObject/CompositeObject.hpp"

namespace GameWorld {

class Boundary : public CompositeObject {
   public:
    Boundary(float x1 = -300.0F, float x2 = 300.0F, float y1 = -300.0F, float y2 = 300.0F);
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
    float m_X1;
    float m_X2;
    float m_Y1;
    float m_Y2;
};

}  // namespace GameWorld
