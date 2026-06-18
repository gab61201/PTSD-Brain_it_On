#pragma once

#include <vector>

#include "Level/PassCondition/PassCondition.hpp"

enum class CountComparison {
    EXACTLY,
    AT_LEAST,
    AT_MOST
};

class CountObjectsInSensorPass : public PassCondition {
   public:
    CountObjectsInSensorPass(
        std::vector<b2ShapeId> objects,
        b2ShapeId sensor,
        int requiredCount,
        int duration = 0,
        CountComparison comparison = CountComparison::EXACTLY);
    ~CountObjectsInSensorPass() = default;

   private:
    void OnContactEvent(b2ShapeId shapeA, b2ShapeId shapeB, TriggerType triggerType) override;
    bool EvaluateCount() const;

    std::vector<b2ShapeId> m_Objects;
    b2ShapeId m_Sensor = b2_nullShapeId;
    int m_RequiredCount = 0;
    CountComparison m_Comparison = CountComparison::EXACTLY;
    std::vector<b2ShapeId> m_ObjectsInside;
};
