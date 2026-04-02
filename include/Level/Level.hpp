#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <memory>

#include "Level/LevelData.hpp"
#include "Level/LevelHUD.hpp"
#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

class Level {
   public:
    Level(LevelId levelId);

    ~Level() = default;

    void Reset();

    void Update();  // 更新畫面

    float GetRemainingTime() const { return std::max(0.0f, m_Timeout - m_Time); }

    enum class State {
        WAITING,
        DRAWING,
        PLAYING,
        FINISHED
    };

    LevelId GetLevelId() const { return m_LevelId; }
    State GetState() const { return m_state; }

   private:
    void Waiting();
    void Drawing();
    void Playing();
    void Finished();

    LevelId m_LevelId;
    State m_state = State::WAITING;
    float m_Time = 0.0F;                    // 遊戲進行時間
    float m_Timeout;                        // 遊戲限制時間
    std::shared_ptr<GameWorld::PhysicalWorld> m_World;
    std::vector<PassCondition> m_pass_conditions;
    std::unique_ptr<LevelHUD> m_HUD;
};

#endif
