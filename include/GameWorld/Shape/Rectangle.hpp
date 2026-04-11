#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "GameWorld/Shape/Shape.hpp"

namespace GameWorld {

class Rectangle : public Shape {
   public:
    Rectangle(const glm::vec2& size, const glm::vec2& relativePosition, bool isSensor = false);

    void AttachToBody(b2BodyId body) override;
};

}  // namespace GameWorld

#endif  // RECTANGLE_HPP
