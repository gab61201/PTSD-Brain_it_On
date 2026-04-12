#ifndef CAPSULE_HPP
#define CAPSULE_HPP

#include "GameWorld/Shape/Shape.hpp"

namespace GameWorld {

class Capsule : public Shape {
public:
    // 建構子：兩點（膠囊形兩端圓心）+ 寬度
    static std::shared_ptr<Capsule> CreateFromPoints(
        const glm::vec2& pointA, 
        const glm::vec2& pointB, 
        float width, 
        bool isSensor = false
    );
    
    // 建構子：中心點 + size(長，寬) + 方向角度
    static std::shared_ptr<Capsule> CreateFromCenter(
        const glm::vec2& center, 
        const glm::vec2& size, 
        float angle,
        bool isSensor = false
    );

    void AttachToBody(b2BodyId body) override;
};

}  // namespace GameWorld

#endif  // CAPSULE_HPP
