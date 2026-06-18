#pragma once

#include <box2d/box2d.h>

#include <variant>

#include "Util/GameObject.hpp"

namespace GameWorld {

enum class ShapeColor {
    White,
    Orange,
    Red,
    Gray,
    Transparent,
    PositiveMagnet,
    NegativeMagnet,
};

class Shape {
   public:
    Shape() = default;

    Shape(std::variant<glm::vec2, float> m_Size,
          const glm::vec2& relativePosition,
          float relativeRotation,
          ShapeColor color = ShapeColor::White,
          bool isSensor = false,
          bool outline = true,
          bool isForbidden = false);

    virtual ~Shape() = default;

    // 創建對應的 b2Shape 實例以及 GameObject 實例並將形狀附加到 b2Body 上
    virtual void AttachToBody(b2BodyId body) = 0;

    // 更新 m_Visual 的世界座標和旋轉角度
    virtual void Update(glm::vec2 ParentObjectPosition, float ParentObjectRotation);

    // 獲取 b2ShapeId
    b2ShapeId Getb2ShapeId() const { return m_b2ShapeId; }

    // 獲取 Util::GameObject
    std::shared_ptr<Util::GameObject> GetVisual() const { return m_Visual; }

    // 是否為禁止繪畫區
    bool IsForbidden() const { return m_IsForbidden; }

    // 獲取顏色
    ShapeColor GetColor() const { return m_Color; }

   protected:
    // 圖像表示 (Util::GameObject)
    std::shared_ptr<Util::GameObject> m_Visual = std::make_shared<Util::GameObject>();

    // 物體視覺描邊
    std::shared_ptr<Util::GameObject> m_OutlineVisual = nullptr;

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

    // 是否為禁止繪畫區（是 sensor，且阻止玩家在該區域繪畫）
    bool m_IsForbidden;

    // 形狀顏色
    ShapeColor m_Color;

};

}  // namespace GameWorld
