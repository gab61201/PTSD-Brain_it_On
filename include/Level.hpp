#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

enum class LevelType {
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
};

class Level {
   public:
    ~Level() = default;

    // 生成關卡物件
    void Initilalize();

    // 開始計時、物理模擬
    void Start();

    // 更新畫面
    void Update();

    // 結算畫面
    void End();

   private:
    // std::vector<PhysicalObject> m_PhysicalObjects;

    // 遊戲進行時間
    float m_Time;

    // 遊戲限制時間
    float m_Timeout;

    // 過關條件
    // PassCondition m_PassCondition;
};

class LevelManager {
};

#endif
