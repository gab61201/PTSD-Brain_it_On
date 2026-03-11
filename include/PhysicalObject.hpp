#ifndef PHYSICAL_OBJECT_HPP
#define PHYSICAL_OBJECT_HPP
#include "Util/GameObject.hpp"

class PhysicalObject {
public:
    PhysicalObject();
    // PhysicalObject(Util::Image);
    virtual ~PhysicalObject() = default;

    virtual void render() = 0;

};

#endif
