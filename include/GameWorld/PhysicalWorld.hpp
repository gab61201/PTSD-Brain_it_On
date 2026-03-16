#ifndef PHYSICAL_WORLD_HPP
#define PHYSICAL_WORLD_HPP

#include "GameWorld/CompositeObject.hpp"

class PhysicalWorld {
   public:
    PhysicalWorld(std::vector<std::shared_ptr<CompositeObject>> compositeObjects);
    ~PhysicalWorld() = default;

    // 更新碰撞並渲染
    void Update();

   private:
    // 玩家繪圖
    void Playing();
    void PlayerDrawObject();
    void Pause();

    enum class state {
        PAUSE,
        PLAYING,
        PLAYER_DRAWING
    };
    state m_state = state::PAUSE;
    b2World m_b2World;
    std::vector<std::shared_ptr<CompositeObject>> m_CompositeObject;
};

#endif  // WORLD_HPP
