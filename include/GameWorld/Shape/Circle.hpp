#ifndef GAMEWORLD_CIRCLE_HPP
#define GAMEWORLD_CIRCLE_HPP

#include "GameWorld/Shape/Shape.hpp"

namespace GameWorld {

class Circle : public Shape {
   public:
    Circle(float diameter, const glm::vec2& relativePosition, bool isSensor = false);

    void AttachToBody(b2BodyId body) override;
};

}  // namespace GameWorld

#endif  // GAMEWORLD_CIRCLE_HPP