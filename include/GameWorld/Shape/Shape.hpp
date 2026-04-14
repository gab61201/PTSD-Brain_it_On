#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <box2d/box2d.h>

#include <variant>

#include "GameWorld/CoordinateHelper.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

namespace GameWorld {

class Shape {
   public:
    Shape() = default;

    Shape(std::variant<glm::vec2, float> m_Size,
        const glm::vec2& relativePosition,
        float relativeRotation,
        bool isSensor = false
    );

    virtual ~Shape() = default;

    // 創建對應的 b2Shape 實例以及 GameObject 實例並將形狀附加到 b2Body 上
    virtual void AttachToBody(b2BodyId body) = 0;

    // 更新 m_Visual 的世界座標和旋轉角度
    virtual void Update(glm::vec2 ParentObjectPosition, float ParentObjectRotation);

    // 獲取 b2ShapeId
    b2ShapeId Getb2ShapeId() const { return m_b2ShapeId; }

    // 獲取 Util::GameObject
    std::shared_ptr<Util::GameObject> GetVisual() const { return m_Visual; }

   protected:
    // 圖像表示 (Util::GameObject)
    std::shared_ptr<Util::GameObject> m_Visual = std::make_shared<Util::GameObject>();

    // 形狀實例
    b2ShapeId m_b2ShapeId = b2_nullShapeId;

    // 大小，對於圓形是直徑，對於矩形是寬高
    std::variant<glm::vec2, float> m_Size;

    // 相對於父物件的位置
    glm::vec2 m_RelativePosition;

    // 相對於父物件的旋轉角度（以弧度為單位）
    float m_RelativeRotation;

    // 是否為感測器（即不會產生物理碰撞，但仍能觸發碰撞事件）
    bool m_IsSensor;

    // 圖片快取 (Image Cache)
    static Util::AssetStore<std::shared_ptr<Util::Image>> s_ImageCache;
};

}  // namespace GameWorld

#endif  // SHAPE_HPP
