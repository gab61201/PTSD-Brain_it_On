#ifndef PHYSICAL_RECTANGLE_HPP
#define PHYSICAL_RECTANGLE_HPP

#include "Physics/PhysicalObject.hpp"

class PhysicsWorld;

class PhysicalRectangle : public PhysicalObject {
public:
    static std::shared_ptr<PhysicalRectangle> Create(
        PhysicsWorld& world,    // 物理世界
        glm::vec2 posPixels,    // PTSD 像素座標
        glm::vec2 sizePixels,   // 長寬像素
        float rotationRadians,  // 旋轉（弧度）
        bool isDynamic          // 會不會動
    );
};

#endif // PHYSICAL_RECTANGLE_HPP
