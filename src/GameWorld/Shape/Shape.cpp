#include "GameWorld/Shape/Shape.hpp"
#include "Constants.hpp"

namespace GameWorld {

Shape::Shape(std::variant<glm::vec2, float> m_Size, const glm::vec2& relativePosition, float relativeRotation, ShapeColor color, bool isSensor, bool outline, bool isForbidden)
    : m_Size(m_Size),
      m_RelativePosition(relativePosition),
      m_RelativeRotation(relativeRotation),
      m_IsSensor(isSensor),
      m_IsForbidden(isForbidden),
      m_Color(color) {
        m_Visual->SetZIndex(Layer::Shape);
        if (outline) {
            m_OutlineVisual = std::make_shared<Util::GameObject>();
            m_OutlineVisual->SetZIndex(Layer::ShapeOutLine);
            m_Visual->AddChild(m_OutlineVisual);
        }
    }

void Shape::Update(glm::vec2 ParentObjectPosition, float ParentObjectRotation) {
    if (!m_Visual) return;
    // 1. 計算絕對旋轉角度 = 父物件目前的旋轉 + 自己的相對旋轉
    float globalRotation = ParentObjectRotation + m_RelativeRotation;

    // 2. 計算「公轉」後的相對座標 (2D 旋轉矩陣)
    // 必須使用父物件的旋轉角度 (ParentObjectRotation) 來進行公轉
    // 注意：C++ 的 std::cos / std::sin 接收的是弧度 (Radians)
    float cosTheta = std::cos(ParentObjectRotation);
    float sinTheta = std::sin(ParentObjectRotation);

    glm::vec2 rotatedLocalPos;
    rotatedLocalPos.x = m_RelativePosition.x * cosTheta - m_RelativePosition.y * sinTheta;
    rotatedLocalPos.y = m_RelativePosition.x * sinTheta + m_RelativePosition.y * cosTheta;

    // 3. 計算出在螢幕上的最終絕對座標 (父物件的世界座標 + 旋轉後的相對偏移量)
    glm::vec2 globalPosition = ParentObjectPosition + rotatedLocalPos;

    // 4. 同步資料給你的 PTSD GameObject
    m_Visual->m_Transform.translation = globalPosition;
    m_Visual->m_Transform.rotation = globalRotation;

    // 5. 同步描邊物件 (如果有)
    if (m_OutlineVisual) {
        m_OutlineVisual->m_Transform.translation = globalPosition;
        m_OutlineVisual->m_Transform.rotation = globalRotation;
    }
}


}  // namespace GameWorld