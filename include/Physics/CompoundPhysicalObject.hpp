#ifndef COMPOUND_PHYSICAL_OBJECT_HPP
#define COMPOUND_PHYSICAL_OBJECT_HPP

#include "Physics/PhysicalObject.hpp"
#include <vector>

class PhysicsWorld;

class CompoundPhysicalObject : public PhysicalObject {
public:
    static std::shared_ptr<CompoundPhysicalObject> Create(
        PhysicsWorld& world,    // 物理世界
        glm::vec2 posPixels,    // PTSD 像素座標
        float rotationRadians,  // 旋轉（弧度）
        bool isDynamic          // 會不會動
    );

    // 因為有可能是多個圖形組成，這裡我們重新定義 Sync 邏輯
    void Sync() override;

    // 添加不同的子物件 (碰撞體 + 視覺)
    void AddCircle(PhysicsWorld& world, glm::vec2 localOffsetPixels, float radiusPixels, const std::string& imagePath = RESOURCE_DIR "/Images/circle.png");
    void AddRectangle(PhysicsWorld& world, glm::vec2 localOffsetPixels, glm::vec2 sizePixels, float localRotationRadians, const std::string& imagePath = RESOURCE_DIR "/Images/square.png");

private:
    std::vector<std::shared_ptr<Util::GameObject>> m_ChildVisuals;
    std::vector<glm::vec2> m_ChildOffsets; // Local offset from the main body
    std::vector<float> m_ChildLocalRotations; // Local rotation offset
};

#endif // COMPOUND_PHYSICAL_OBJECT_HPP
