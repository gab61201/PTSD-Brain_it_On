#include "GameWorld/Shape/Shape.hpp"

namespace GameWorld {

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
}

Util::AssetStore<std::shared_ptr<Util::Image>> Shape::s_ImageCache{
    [](const std::string& filepath) {
        return std::make_shared<Util::Image>(filepath);
    }};

}  // namespace GameWorld