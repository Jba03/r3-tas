#ifndef stPhysicalObject_h
#define stPhysicalObject_h

#include "library.hh"
#include "stCollideSet.hh"

struct stPhysicalObject
{
    readonly pointer<> visualSet;
    readonly pointer<stPhysicalCollideSet> physicalCollideset;
    readonly pointer<> visualBoundingVolume;
    readonly pointer<> collideBoundingVolume;
};

#endif /* stPhysicalObject_h */
