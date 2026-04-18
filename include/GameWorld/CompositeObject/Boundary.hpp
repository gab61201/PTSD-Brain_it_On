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
    Boundary(glm::vec2 size = {600.0F, 600.0F},
             glm::vec2 center = {0.0F, 0.0F},
             bool buildTop = true,
             bool buildBottom = true,
             bool buildLeft = true,
             bool buildRight = true);
    ~Boundary() = default;

   private:
};

}  // namespace GameWorld

#endif  // BOUNDARY_HPP
