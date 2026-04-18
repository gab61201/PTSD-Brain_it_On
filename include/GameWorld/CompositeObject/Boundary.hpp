#ifndef BOUNDARY_HPP
#define BOUNDARY_HPP

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "GameWorld/CompositeObject/CompositeObject.hpp"
#include "GameWorld/Shape/Rectangle.hpp"

namespace GameWorld {

class Boundary : public CompositeObject {
public:
   Boundary(float x1, float x2, float y1, float y2);
   ~Boundary() = default;
   bool IsPointInside(glm::vec2 position) const;

private:
   float m_X1 = 0.0F, m_X2 = 0.0F, m_Y1 = 0.0F, m_Y2 = 0.0F;
};

}  // namespace GameWorld

#endif  // BOUNDARY_HPP
