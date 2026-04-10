#ifndef PHYSICS_PHYSICS_HPP
#define PHYSICS_PHYSICS_HPP

#include <box2d/box2d.h>

namespace Physics {

using World = b2World;
using WorldPtr = b2World*;

using Body = b2Body;
using BodyPtr = b2Body*;

using Shape = b2Fixture;
using ShapePtr = b2Fixture*;

using Contact = b2Contact;
using ContactPtr = b2Contact*;

using ContactListener = b2ContactListener;

}  // namespace Physics

#endif  // PHYSICS_PHYSICS_HPP