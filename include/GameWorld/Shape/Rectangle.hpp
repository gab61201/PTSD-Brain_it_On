#ifndef GAMEWORLD_RECTANGLE_HPP
#define GAMEWORLD_RECTANGLE_HPP

#include "GameWorld/Shape/Shape.hpp"

namespace GameWorld {

class Rectangle : public Shape {
   public:
    Rectangle(const glm::vec2& size, const glm::vec2& relativePosition, float relativeRotation = 0.0f, bool isSensor = false);

    void AttachToBody(b2BodyId body) override;
};

}  // namespace GameWorld

#endif  // GAMEWORLD_RECTANGLE_HPP
