#ifndef PHYSICAL_CIRCLE_HPP
#define PHYSICAL_CIRCLE_HPP

#include "Physics/PhysicalObject.hpp"

class PhysicsWorld;

class PhysicalCircle : public PhysicalObject {
public:
    static std::shared_ptr<PhysicalCircle> Create(
        PhysicsWorld& world,    // 物理世界
        glm::vec2 posPixels,    // PTSD 像素座標
        float radiusPixels,     // 半徑像素
        float rotationRadians,  // 旋轉（弧度）
        bool isDynamic          // 會不會動
    );
};

#endif // PHYSICAL_CIRCLE_HPP
