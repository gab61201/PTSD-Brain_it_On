#include "GameWorld/MagnetObject.hpp"

#include "Physics/Physics.hpp"

#include <cmath>
#include <glm/fwd.hpp>

#include "GameWorld/BaseObject.hpp"

namespace GameWorld {

MagnetObject::MagnetObject(
    std::vector<std::shared_ptr<BaseObject>> baseObjects,
    BodyType bodyType,
    glm::vec2 position,
    float rotation,
    float magnetism)
    : CompositeObject(std::move(baseObjects), bodyType, position, rotation),
      m_Magnetism(magnetism) {
    // 磁場範圍與磁力成正比（直徑 = |magnetism| * 10 像素）
    float fieldSize = std::abs(magnetism) * 10.0f;
    BaseObject magneticField(GameWorld::ShapeType::CIRCLE,
                             glm::vec2(fieldSize, fieldSize),
                             glm::vec2(0.0f, 0.0f),
                             0.0f,
                             true);
    m_BaseObjects.push_back(std::make_shared<BaseObject>(magneticField));
}

// ==========================================
// 掛載到物理世界，並在 Body 上標記 this 指標
// 這樣其他磁鐵就能透過 UserData 辨識這個 Body 是磁鐵
// ==========================================
void MagnetObject::AttachToWorld(Physics::WorldPtr world) {
    CompositeObject::AttachToWorld(world);

    if (m_Body != nullptr) {
        // 將 this 指標存入 Box2D 的 UserData
        // 其他磁鐵在 Update() 中可以用這個來辨識「對方是不是磁鐵」
        m_Body->GetUserData().pointer =
            reinterpret_cast<uintptr_t>(this);
    }
}

void MagnetObject::Update() {
    // 1. 先呼叫父類的 Update()，更新座標與子零件的圖片位置
    CompositeObject::Update();

    // 2. 如果骨架還沒生成，就無法施加力
    if (m_Body == nullptr) {
        return;
    }

    // 取得磁鐵自身的位置（公尺座標）
    b2Vec2 magnetPos = m_Body->GetPosition();

    // 3. 遍歷這個 Body 的所有接觸 (Contact)
    for (b2ContactEdge* ce = m_Body->GetContactList(); ce != nullptr;
         ce = ce->next) {
        b2Contact* contact = ce->contact;

        // 只處理正在接觸中的
        if (!contact->IsTouching()) {
            continue;
        }

        // 確認這個接觸至少有一方是 Sensor（我們的磁場）
        bool fixtureAIsSensor = contact->GetFixtureA()->IsSensor();
        bool fixtureBIsSensor = contact->GetFixtureB()->IsSensor();
        if (!fixtureAIsSensor && !fixtureBIsSensor) {
            continue;
        }

        // 取得「另一個物體」的 Body
        b2Body* otherBody = ce->other;

        // ====================================================
        // 關鍵：只影響其他磁鐵！
        // 如果 otherBody 的 UserData 是 0，代表它不是 MagnetObject
        // ====================================================
        uintptr_t otherData = otherBody->GetUserData().pointer;
        if (otherData == 0) {
            continue;  // 不是磁鐵，跳過
        }

        // 將 UserData 還原成 MagnetObject 指標，讀取對方的磁力值
        auto* otherMagnet = reinterpret_cast<MagnetObject*>(otherData);
        float otherMagnetism = otherMagnet->GetMagnetism();

        // 4. 計算方向（從對方指向自己）
        b2Vec2 otherPos = otherBody->GetPosition();
        b2Vec2 direction = magnetPos - otherPos;
        float distanceSq = direction.LengthSquared();

        // 避免距離太近導致力過大
        constexpr float kMinDistanceSq = 1.0f;  // 提高最小距離上限
        if (distanceSq < kMinDistanceSq) {
            distanceSq = kMinDistanceSq;
        }

        // 正規化方向向量
        float distance = std::sqrt(distanceSq);
        direction.x /= distance;
        direction.y /= distance;

        // 5. 計算力的大小與方向
        //    product < 0 → 異極相吸（力指向磁鐵，即 direction 方向）
        //    product > 0 → 同極相斥（力背離磁鐵，即 -direction 方向）
        float product = m_Magnetism * otherMagnetism;
        float forceMagnitude = std::abs(product) / distanceSq;

        // 限制最大力，避免碰撞瞬間力爆炸
        constexpr float kMaxForce = 40.0f;
        if (forceMagnitude > kMaxForce) {
            forceMagnitude = kMaxForce;
        }

        // product < 0 時 sign = 1（吸引，direction 指向自己）
        // product > 0 時 sign = -1（排斥，direction 反轉）
        float sign = (product < 0) ? 1.0f : -1.0f;

        b2Vec2 force(sign * direction.x * forceMagnitude,
                     sign * direction.y * forceMagnitude);

        // 6. 對另一個磁鐵的質心施加力
        otherBody->ApplyForceToCenter(force, true);
    }
}

}  // namespace GameWorld
