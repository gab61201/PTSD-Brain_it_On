#pragma once

#include "GameWorld/Shape/Shape.hpp"

namespace GameWorld {

class Rectangle : public Shape {
   public:
    Rectangle(const glm::vec2& size, const glm::vec2& relativePosition, float relativeRotation = 0.0f, ShapeColor color = ShapeColor::White, bool isSensor = false, bool outline = true, bool isForbidden = false);

    void AttachToBody(b2BodyId body) override;
};

}  // namespace GameWorld
