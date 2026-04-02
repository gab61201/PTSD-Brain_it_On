#ifndef LEVEL_HUD_HPP
#define LEVEL_HUD_HPP

#include <memory>
#include <string>

#include "Level/LevelData.hpp"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"

class LevelHUD {
   public:
    LevelHUD(LevelId levelId, const std::string& targetText);

    /// 每幀更新計時器顯示
    void UpdateTimer(float remainingTime);

    /// 隱藏提示文字（玩家開始繪圖時呼叫）
    void HideTarget();

    /// 重置 HUD（重新開始關卡時呼叫）
    void Reset(const std::string& targetText);

    /// 繪製所有 HUD 元素
    void Update();

   private:
    Util::Renderer m_Renderer;
    std::shared_ptr<Util::Text> m_TimerText;
    std::shared_ptr<Util::GameObject> m_TimerObject;
    std::shared_ptr<Util::GameObject> m_TargetTextObject;
};

#endif
