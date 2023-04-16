#ifndef stPhysicalObject_h
#define stPhysicalObject_h

#include "structure.h"

typedef struct stPhysicalObject tdstPhysicalObject, tdstPO;

struct stPhysicalObject
{
    readonly pointer visualSet;
    readonly pointer physicalCollideset;
    readonly pointer visualBoundingVolume;
    readonly pointer collideBoundingVolume;
};

#endif /* stPhysicalObject_h */
