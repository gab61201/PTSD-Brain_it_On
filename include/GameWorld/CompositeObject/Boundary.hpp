#pragma once

#include <glm/glm.hpp>

#include "GameWorld/CompositeObject/CompositeObject.hpp"

namespace GameWorld {

class Boundary : public CompositeObject {
   public:
    Boundary(float x1, float x2, float y1, float y2);
   ~Boundary() = default;
    bool IsPointInside(glm::vec2 position) const;

    const Shape& GetLeftWall() const;
    const Shape& GetRightWall() const;
    const Shape& GetTopWall() const;
    const Shape& GetBottomWall() const;

    private:
    float m_X1 = 0.0F;
    float m_X2 = 0.0F;
    float m_Y1 = 0.0F;
    float m_Y2 = 0.0F;
};

}  // namespace GameWorld
