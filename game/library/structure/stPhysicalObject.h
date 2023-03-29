#ifndef stPhysicalObject_h
#define stPhysicalObject_h

#include "structure.h"

struct stPhysicalObject {
    readonly pointer visualSet;
    readonly pointer physicalCollideset;
    readonly pointer visualBoundingVolume;
    readonly pointer collideBoundingVolume;
} typedef tdstPhysicalObject, tdstPO;

#endif /* stPhysicalObject_h */
