#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "GameWorld/Shape/Shape.hpp"

namespace GameWorld {

class Circle : public Shape {
   public:
    Circle(float diameter, const glm::vec2& relativePosition, bool isSensor = false);

    void AttachToBody(b2BodyId body) override;

    
};

}  // namespace GameWorld

#endif  // CIRCLE_HPP