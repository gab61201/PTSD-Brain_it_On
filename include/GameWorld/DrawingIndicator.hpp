#ifndef DRAWING_INDICATOR_HPP
#define DRAWING_INDICATOR_HPP

#include "Util/AssetStore.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"
#include "Util/GameObject.hpp"

namespace GameWorld {

class DrawingIndicator {
public:
    DrawingIndicator();

    // 在兩點之間繪製一條線段
    void DrawLine(const glm::vec2& p1, const glm::vec2& p2);

    // 更新繪製指示器
    void Update();

private:
    static Util::AssetStore<std::shared_ptr<Util::Image>> s_ImageCache;
    Util::Renderer m_Renderer;
    std::shared_ptr<Util::GameObject> m_P1;
    std::shared_ptr<Util::GameObject> m_P2;
    std::shared_ptr<Util::GameObject> m_Line;
};

} // namespace GameWorld

#endif  // DRAWING_INDICATOR_HPP