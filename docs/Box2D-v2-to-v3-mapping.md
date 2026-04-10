# Box2D v2 -> v3 API 映射表

本文件整理專案遷移時最常用的 Box2D API 對照，方便後續維護與除錯。

## 核心型別映射

| v2.x                | v3.x              | 說明                              |
| ------------------- | ----------------- | --------------------------------- |
| `b2World`           | `b2WorldId`       | 世界由 class instance 改為 handle |
| `b2Body*`           | `b2BodyId`        | Body 由指標改為 handle            |
| `b2Fixture*`        | `b2ShapeId`       | Fixture 概念改為 Shape handle     |
| `b2ContactListener` | `b2ContactEvents` | 改為每幀讀取接觸事件緩衝          |

## 世界生命週期

| v2.x                               | v3.x                                                                           |
| ---------------------------------- | ------------------------------------------------------------------------------ |
| `b2World world(gravity)`           | `b2WorldDef def = b2DefaultWorldDef(); def.gravity = ...; b2CreateWorld(&def)` |
| `delete world` (或物件析構)        | `b2DestroyWorld(worldId)`                                                      |
| `world.Step(dt, velIter, posIter)` | `b2World_Step(worldId, dt, subStepCount)`                                      |

## Body 與 Shape 建立

| v2.x                               | v3.x                                                     |
| ---------------------------------- | -------------------------------------------------------- |
| `world->CreateBody(&bodyDef)`      | `b2CreateBody(worldId, &bodyDef)`                        |
| `body->CreateFixture(&fixtureDef)` | `b2CreateCircleShape(...)` / `b2CreatePolygonShape(...)` |
| `body->SetType(type)`              | `b2Body_SetType(bodyId, type)`                           |
| `body->GetPosition()`              | `b2Body_GetPosition(bodyId)`                             |
| `body->GetAngle()`                 | `b2Body_GetAngle(bodyId)`                                |

## 查詢與事件

| v2.x                               | v3.x                                                       |
| ---------------------------------- | ---------------------------------------------------------- |
| `b2QueryCallback`                  | `b2World_OverlapAABB` + callback/context                   |
| `b2RayCastCallback`                | `b2World_CastRay` + callback/context                       |
| `BeginContact/EndContact` override | 讀取 `b2World_GetContactEvents(worldId)` 的 begin/end 陣列 |

## 專案內抽象層對應

專案使用 [include/Physics/Physics.hpp](../include/Physics/Physics.hpp) 封裝常用型別，建議優先使用：

- `Physics::World` / `Physics::WorldPtr`
- `Physics::Body` / `Physics::BodyPtr`
- `Physics::Shape` / `Physics::ShapePtr`
- `Physics::ContactEvents`

## 已知限制與注意事項

1. 事件資料為「每幀快照」，不要跨幀持有 event 陣列指標。
2. `b2_null*Id` 需明確判斷，不能再以 `nullptr` 表示未掛載狀態。
3. ray cast 語義為 `origin + translation`，不是 v2 常見的 `point1 -> point2` 介面。
4. 若需加新功能，優先維持在 `Physics` 抽象層內封裝，再向上層暴露。
