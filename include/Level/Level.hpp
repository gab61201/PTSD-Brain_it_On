#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Level/LevelData.hpp"
#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

class Level {
   public:
    // 生成關卡物件
    Level() = default;
    Level(LevelId);

    ~Level() = default;

    void Reset();

    void Update();  // 更新畫面

    float GetRemainingTime() const { return std::max(0.0f, m_Timeout - m_Time); }

   private:
    void Waiting();
    void Drawing();
    void Playing();
    void Finished();

    enum class state {
        WAITING,
        DRAWING,
        PLAYING,
        FINISHED
    };
    LevelId m_LevelId;
    state m_state = state::WAITING;
    float m_Time = 0.0F;                    // 遊戲進行時間
    float m_Timeout;                        // 遊戲限制時間
    float m_pass_condition_check_duration;  // 通關檢測持續多久才算過關
    std::shared_ptr<GameWorld::PhysicalWorld> m_World;
    std::vector<PassCondition> m_pass_conditions;
    std::shared_ptr<Util::GameObject> m_TargetText;
    std::shared_ptr<Util::GameObject> m_TimerObject;
    std::shared_ptr<Util::Text> m_TimerText;
    std::shared_ptr<Util::GameObject> m_LevelNumberText;
};

#endif
