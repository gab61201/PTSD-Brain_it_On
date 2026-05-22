#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <memory>

#include "Level/LevelData.hpp"
#include "Level/LevelHUD.hpp"

struct LevelResultData {
    LevelId levelId = LevelId::LEVEL_1;
    bool passed = false;
    float goalTime = 0.0f;
    float solvedTime = 0.0f;
    int goalStroke = 0;
    int usedStroke = 0;
    std::string screenshotFilename;
};

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
    void Save();

   private:
    void Waiting();
    void Drawing();
    void Playing();

    LevelId m_LevelId;
    State m_state = State::WAITING;
    float m_Time = 0.0F;  // 遊戲進行時間
    float m_Timeout;      // 遊戲限制時間
    int m_StrokeLimit;
    std::shared_ptr<GameWorld::PhysicalWorld> m_World;
    std::shared_ptr<PassCondition> m_PassCondition;
    std::unique_ptr<LevelHUD> m_HUD;
};

#endif
