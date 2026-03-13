#ifndef COORDINATE_HELPER_HPP
#define COORDINATE_HELPER_HPP

#include <box2d/box2d.h>
#include <glm/vec2.hpp>

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

#endif  // COORDINATE_HELPER_HPP
