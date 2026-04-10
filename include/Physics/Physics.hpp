#ifndef PHYSICS_PHYSICS_HPP
#define PHYSICS_PHYSICS_HPP

#include <box2d/box2d.h>

namespace Physics {

using World = b2WorldId;
using WorldPtr = b2WorldId;

using Body = b2BodyId;
using BodyPtr = b2BodyId;

using Shape = b2ShapeId;
using ShapePtr = b2ShapeId;

using ContactEvents = b2ContactEvents;

}  // namespace Physics

#endif  // PHYSICS_PHYSICS_HPP