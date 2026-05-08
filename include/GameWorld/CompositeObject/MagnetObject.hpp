#ifndef MAGNET_OBJECT_HPP
#define MAGNET_OBJECT_HPP

#include "GameWorld/CompositeObject/CompositeObject.hpp"

namespace GameWorld {

class MagnetObject : public CompositeObject {
   public:
    MagnetObject(
        std::vector<std::shared_ptr<Shape>> shapes,
        BodyType bodyType = BodyType::STATIC,
        glm::vec2 position = {0.0F, 0.0F},
        float rotation = 0.0F,
        float magnetism = 0.0F);

    void AttachToWorld(b2WorldId world) override;
    void Update() override;

    float GetMagnetism() const { return m_Magnetism; }

   private:
    float m_Magnetism;
    b2WorldId m_WorldId = b2_nullWorldId;
};

}  // namespace GameWorld

#endif  // MAGNET_OBJECT_HPP
