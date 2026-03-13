#include "Physics/CompoundPhysicalObject.hpp"

#include <cmath>

#include <box2d/box2d.h>

#include "Physics/CoordinateHelper.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Util/GameObject.hpp"

// --- 工廠方法 ---

std::shared_ptr<CompoundPhysicalObject> CompoundPhysicalObject::Create(
    PhysicsWorld& world, glm::vec2 posPixels, float rotationRadians, bool isDynamic) {
    auto obj = std::make_shared<CompoundPhysicalObject>();

    // 建立一個空的 body（之後再透過 AddCircleChild / AddBoxChild 加 fixture）
    obj->m_Body = world.CreateEmptyBody(posPixels, rotationRadians, isDynamic);

    // root visual — 本身可以不畫東西，只當作子物件的 parent node
    obj->m_Visual = std::make_shared<Util::GameObject>();
    obj->m_Visual->m_Transform.translation = posPixels;
    obj->m_Visual->m_Transform.rotation = rotationRadians;

    return obj;
}

// --- 加入子物件 ---

void CompoundPhysicalObject::AddCircleChild(
    PhysicsWorld& world,
    std::shared_ptr<Util::GameObject> visual,
    glm::vec2 localOffsetPixels,
    float radiusPixels,
    float localRotation) {
    // 在 body 上加一個圓形 fixture（帶本地偏移）
    world.AddCircleFixture(m_Body, localOffsetPixels, radiusPixels);

    // 記錄子物件資訊
    m_ChildInfos.push_back({visual, localOffsetPixels, localRotation});

    // 加入 PTSD 的 parent-child 樹（讓 Renderer 能走訪到）
    m_Visual->AddChild(visual);
}

void CompoundPhysicalObject::AddBoxChild(
    PhysicsWorld& world,
    std::shared_ptr<Util::GameObject> visual,
    glm::vec2 localOffsetPixels,
    glm::vec2 halfSizePixels,
    float localRotation) {
    // 在 body 上加一個矩形 fixture（帶本地偏移與旋轉）
    world.AddBoxFixture(m_Body, localOffsetPixels, halfSizePixels, localRotation);

    m_ChildInfos.push_back({visual, localOffsetPixels, localRotation});

    m_Visual->AddChild(visual);
}

// --- 每幀同步 ---

void CompoundPhysicalObject::Sync() {
    if (m_Body == nullptr || m_Visual == nullptr) {
        return;
    }

    // 取得 body 的世界座標與角度
    glm::vec2 bodyPosPixels = MetersToPixels(m_Body->GetPosition());
    float bodyAngle = m_Body->GetAngle();

    // 同步 root visual
    m_Visual->m_Transform.translation = bodyPosPixels;
    m_Visual->m_Transform.rotation = bodyAngle;

    // 同步所有子物件：worldPos = bodyPos + Rotate(localOffset, bodyAngle)
    float cosA = std::cos(bodyAngle);
    float sinA = std::sin(bodyAngle);

    for (auto& child : m_ChildInfos) {
        glm::vec2 rotatedOffset = {
            child.localOffset.x * cosA - child.localOffset.y * sinA,
            child.localOffset.x * sinA + child.localOffset.y * cosA};

        child.visual->m_Transform.translation = bodyPosPixels + rotatedOffset;
        child.visual->m_Transform.rotation = bodyAngle + child.localRotation;
    }
}

// --- Getter ---

const std::vector<ChildInfo>& CompoundPhysicalObject::GetChildInfos() const {
    return m_ChildInfos;
}
