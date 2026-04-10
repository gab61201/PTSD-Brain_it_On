#ifndef GAMEWORLD_COORDINATE_HELPER_HPP
#define GAMEWORLD_COORDINATE_HELPER_HPP

#include "Physics/Physics.hpp"

#include <glm/vec2.hpp>

namespace GameWorld {

constexpr float kPixelsPerMeter = 50.0F;

inline b2Vec2 PixelsToMeters(glm::vec2 pixels) {
    return {pixels.x / kPixelsPerMeter, pixels.y / kPixelsPerMeter};
}

inline float PixelsToMeters(float pixels) {
    return pixels / kPixelsPerMeter;
}

inline glm::vec2 MetersToPixels(b2Vec2 meters) {
    return {meters.x * kPixelsPerMeter, meters.y * kPixelsPerMeter};
}

}  // namespace GameWorld

#endif  // GAMEWORLD_COORDINATE_HELPER_HPP
