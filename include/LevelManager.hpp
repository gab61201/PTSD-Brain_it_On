#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

class LevelManager {
   public:
    enum class LevelState {
        LEVEL_1,
        LEVEL_2,
        LEVEL_3,
    };
    LevelState m_CurrentLevel = LevelState::LEVEL_1;

    void Start();

    void Update();

   private:
};

#endif
