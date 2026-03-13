#ifndef COMPOUND_PHYSICAL_OBJECT_HPP
#define COMPOUND_PHYSICAL_OBJECT_HPP

#include <memory>
#include <vector>

#include <glm/vec2.hpp>

#include "Physics/PhysicalObject.hpp"

class PhysicsWorld;

/**
 * @brief 記錄每個子視覺物件相對於 body 重心的本地偏移
 */
struct ChildInfo {
    std::shared_ptr<Util::GameObject> visual;
    glm::vec2 localOffset;     // 相對 body 位置的偏移 (pixels)
    float localRotation;       // 相對 body 的旋轉 (radians)
};

/**
 * @class CompoundPhysicalObject
 * @brief 複合物理物件：單一 b2Body (多個 b2Fixture) + 多個 PTSD 子視覺物件
 *
 * 呼叫 Sync() 時，會把 Box2D body 的世界座標/旋轉
 * 傳播到 root 與所有子視覺物件。
 */
class CompoundPhysicalObject : public PhysicalObject {
   public:
    /**
     * @brief 建立一個空的 compound 物件（只有 body，還沒有 fixture）
     *
     * @param world 物理世界
     * @param posPixels body 初始位置 (PTSD 像素座標)
     * @param rotationRadians body 初始旋轉 (弧度)
     * @param isDynamic 是否為動態物體
     */
    static std::shared_ptr<CompoundPhysicalObject> Create(
        PhysicsWorld& world,
        glm::vec2 posPixels,
        float rotationRadians,
        bool isDynamic);

    /**
     * @brief 加入一個圓形子物件 (fixture + visual)
     *
     * @param world 物理世界（用來在 body 上加 fixture）
     * @param visual 子視覺物件
     * @param localOffsetPixels 相對 body 中心的偏移 (pixels)
     * @param radiusPixels 圓形半徑 (pixels)
     * @param localRotation 相對 body 的旋轉 (radians)
     */
    void AddCircleChild(
        PhysicsWorld& world,
        std::shared_ptr<Util::GameObject> visual,
        glm::vec2 localOffsetPixels,
        float radiusPixels,
        float localRotation = 0.0F);

    /**
     * @brief 加入一個矩形子物件 (fixture + visual)
     *
     * @param world 物理世界
     * @param visual 子視覺物件
     * @param localOffsetPixels 相對 body 中心的偏移 (pixels)
     * @param halfSizePixels 矩形半寬半高 (pixels)
     * @param localRotation 相對 body 的旋轉 (radians)
     */
    void AddBoxChild(
        PhysicsWorld& world,
        std::shared_ptr<Util::GameObject> visual,
        glm::vec2 localOffsetPixels,
        glm::vec2 halfSizePixels,
        float localRotation = 0.0F);

    /**
     * @brief 同步 Box2D body → root + 所有子視覺物件
     *
     * 每個 child 的世界座標 = bodyPos + Rotate(localOffset, bodyAngle)
     */
    void Sync() override;

    /**
     * @brief 取得所有子物件資訊
     */
    const std::vector<ChildInfo>& GetChildInfos() const;

   private:
    std::vector<ChildInfo> m_ChildInfos;
};

#endif  // COMPOUND_PHYSICAL_OBJECT_HPP
