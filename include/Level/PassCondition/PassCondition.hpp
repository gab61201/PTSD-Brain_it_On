#ifndef PASS_CONDITION_HPP
#define PASS_CONDITION_HPP

#include <box2d/box2d.h>

#include "GameWorld/Shape/Shape.hpp"

// 放一個物體到盒子內
// 將球放到盒子內

// 將球放到任意盒子內
// 分類球體

enum class TriggerType {
    TOUCHING,
    SEPARATED
};

class PassCondition {
   public:
    PassCondition() = default;
    PassCondition(
        TriggerType triggerType,
        int duration);
    virtual ~PassCondition() = default;

    // 檢查是否達成通關條件
    bool Check(b2ContactEvents events);

    // 取得接觸倒數計時器的剩餘時間（無接觸回傳 0）
    int GetContactCountDown() const;

   protected:
    virtual void OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) = 0;
    TriggerType m_TriggerType;
    int m_Duration;
    int m_Timer = 0;
};

#endif