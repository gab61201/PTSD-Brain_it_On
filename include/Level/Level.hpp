#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "LevelData.hpp"

class Level {
   public:
    // 生成關卡物件
    Level() = default;
    Level(LevelId);

    ~Level() = default;

    // 開始計時、物理模擬
    void Start();

    // 更新畫面
    void Update();

    // 結算畫面
    void End();

   private:
    LevelId m_LevelId;

    // 遊戲進行時間
    float m_Time = 0.0F;

    // 遊戲限制時間
    float m_Timeout;
    std::vector<std::shared_ptr<PhysicalObject>> m_objects;
    std::vector<std::shared_ptr<Util::GameObject>> m_banned_areas;
    std::vector<PassCondition> m_pass_conditions;
    float m_pass_condition_check_duration;  // 通關檢測持續多久才算過關
};

#endif
