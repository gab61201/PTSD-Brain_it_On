#pragma once

#include "GameWorld/Shape/Shape.hpp"

namespace GameWorld {

class Circle : public Shape {
   public:
    Circle(float diameter, const glm::vec2& relativePosition, ShapeColor color = ShapeColor::White, bool isSensor = false, bool outline = true, bool isForbidden = false);

    void AttachToBody(b2BodyId body) override;
};

}  // namespace GameWorld