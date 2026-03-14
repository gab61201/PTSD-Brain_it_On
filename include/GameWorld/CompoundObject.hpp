#ifndef GAMEWORLD_COMPOUND_OBJECT_HPP
#define GAMEWORLD_COMPOUND_OBJECT_HPP

#include <glm/vec2.hpp>
#include <memory>
#include <vector>

#include "GameWorld/BaseObject.hpp"
#include "Util/GameObject.hpp"

class b2Body;

namespace GameWorld {

class CompoundObject {
   public:
    // 第二個參數傳入多個 BaseObject 作為子物件
    CompoundObject(Physics& physics, const std::vector<BaseObject>& children);

    void Sync();

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

}  // namespace GameWorld

#endif  // GAMEWORLD_COMPOUND_OBJECT_HPP
