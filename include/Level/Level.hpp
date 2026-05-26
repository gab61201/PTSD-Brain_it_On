#pragma once

#include <memory>

#include "Level/LevelData.hpp"
#include "Level/LevelHUD.hpp"

class Level {
   public:
    explicit Level(LevelId levelId);

    ~Level() = default;

    void Reset();

    void Update();

    float GetRemainingTime() const { return std::max(0.0f, m_Timeout - m_ElapsedTime); }

    enum class State {
        WAITING,
        PLAYING,
        FINISHED
    };

    LevelId GetLevelId() const { return m_LevelId; }
    State GetState() const { return m_State; }
    const LevelResult& GetLastResult() const { return m_LastResult; }
    void Save();

   private:
    void Waiting();
    void Playing();
    void Finished();

    LevelId m_LevelId;
    State m_State = State::WAITING;
    float m_ElapsedTime = 0.0F;
    float m_Timeout;
    int m_StrokeLimit;
    std::shared_ptr<GameWorld::PhysicalWorld> m_World;
    std::shared_ptr<PassCondition> m_PassCondition;
    std::unique_ptr<LevelHUD> m_HUD;
    LevelResult m_LastResult;
};
