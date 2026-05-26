#pragma once

#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"

namespace GameWorld {

class DrawingIndicator {
   public:
    DrawingIndicator();

    // 在兩點之間繪製一條線段
    void DrawLine(const glm::vec2& p1, const glm::vec2& p2);

    // 更新繪製指示器
    void Update();

   private:
    Util::Renderer m_Renderer;
    std::shared_ptr<Util::GameObject> m_P1;
    std::shared_ptr<Util::GameObject> m_P2;
    std::shared_ptr<Util::GameObject> m_Line;
};

}  // namespace GameWorld