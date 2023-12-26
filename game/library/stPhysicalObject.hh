#ifndef stPhysicalObject_h
#define stPhysicalObject_h

#include "library.hh"

typedef struct stPhysicalObject stPhysicalObject, stPO;

struct stPhysicalObject
{
    readonly pointer<> visualSet;
    readonly pointer<> physicalCollideset;
    readonly pointer<> visualBoundingVolume;
    readonly pointer<> collideBoundingVolume;
};

#endif /* stPhysicalObject_h */
