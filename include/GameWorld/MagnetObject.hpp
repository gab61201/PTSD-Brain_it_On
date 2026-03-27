#ifndef MAGNET_OBJECT_HPP
#define MAGNET_OBJECT_HPP

#include "GameWorld/CompositeObject.hpp"

namespace GameWorld {

class MagnetObject : public CompositeObject {
   public:
    // 建構時只需要傳入磁鐵放在哪裡就好
    MagnetObject(
        std::vector<std::shared_ptr<BaseObject>> baseObjects,
        BodyType bodyType = BodyType::DYNAMIC,
        glm::vec2 position = {0.0F, 0.0F},
        float rotation = 0.0F,
        float magnetism = 0.0F);
    ~MagnetObject() = default;

    // 掛載到物理世界時，同時將 this 指標存入 b2Body 的 UserData
    void AttachToWorld(b2World* world);

    // 擴充 Update，除了更新座標，還能每一幀執行磁鐵的相吸邏輯
    void Update() override;

    // 取得磁力值（正值 = N 極，負值 = S 極）
    [[nodiscard]] float GetMagnetism() const { return m_Magnetism; }

   private:
    float m_Magnetism = 0;
};

}  // namespace GameWorld

#endif
