#ifndef GAMEWORLD_COMPOUND_OBJECT_HPP
#define GAMEWORLD_COMPOUND_OBJECT_HPP

#include "GameWorld/BaseObject.hpp"

class b2Body;

class CompoundObject {
   public:
    // 第二個參數傳入多個 BaseObject 作為子物件
    CompoundObject(const std::vector<BaseObject>& children);

    void Update();

    std::vector<std::shared_ptr<Util::GameObject>> GetVisuals() const;
    std::shared_ptr<Util::GameObject> GetRootVisual() const { return m_Visual; }

    glm::vec2 GetPosition() const;
    void SetPosition(glm::vec2 posPixels);
    float GetRotation() const;
    void SetRotation(float angleRadians);

   private:
    b2Body* m_Body = nullptr;
    std::shared_ptr<Util::GameObject> m_Visual;
    std::vector<std::shared_ptr<Util::GameObject>> m_ChildVisuals;
    std::vector<glm::vec2> m_ChildOffsets;     // Local offset from the main body
    std::vector<float> m_ChildLocalRotations;  // Local rotation offset
};

#endif  // GAMEWORLD_COMPOUND_OBJECT_HPP
