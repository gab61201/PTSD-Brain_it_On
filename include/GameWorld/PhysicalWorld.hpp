#ifndef PHYSICAL_WORLD_HPP
#define PHYSICAL_WORLD_HPP

#include "GameWorld/CompositeObject.hpp"

class PhysicalWorld {
   public:
    PhysicalWorld() = default;
    PhysicalWorld(std::vector<std::shared_ptr<CompositeObject>> compositeObjects);
    ~PhysicalWorld() = default;

    // 更新碰撞並渲染
    void Update();
    // 開始遊戲
    void Start();
    // 結束遊戲
    void Stop();

    // 玩家繪圖
    void PlayerDrawObject();

   private:
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
