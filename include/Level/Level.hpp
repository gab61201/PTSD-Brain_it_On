#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "LevelData.hpp"

enum class state {
    WAITING,
    PLAYING,
    FINISHED
};

class Level {
   public:
    // 生成關卡物件
    Level() = default;
    Level(LevelId);

    ~Level() = default;

    void Update();  // 更新畫面

   private:
    LevelId m_LevelId;
    state m_state = state::WAITING;
    void Waiting();     // 等待玩家繪製
    void Playing();     // 遊戲中，循環檢查通關條件
    void Finished();    // 結算畫面
    int m_DrawnObjectCount = 0;  // 物件數量
    float m_Time = 0.0F;    // 遊戲進行時間
    float m_Timeout;        // 遊戲限制時間
    float m_pass_condition_check_duration;  // 通關檢測持續多久才算過關
    std::vector<std::shared_ptr<PhysicalObject>> m_objects;
    std::vector<std::shared_ptr<Util::GameObject>> m_banned_areas;
    std::vector<PassCondition> m_pass_conditions;
};

#endif
