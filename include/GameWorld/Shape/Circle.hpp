#pragma once

#include "GameWorld/Shape/Shape.hpp"

namespace GameWorld {

class Circle : public Shape {
   public:
    Circle(float diameter, const glm::vec2& relativePosition, bool isSensor = false, bool outline = true);

    void AttachToBody(b2BodyId body) override;
};

}  // namespace GameWorld