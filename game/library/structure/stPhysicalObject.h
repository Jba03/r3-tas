#ifndef stPhysicalObject_h
#define stPhysicalObject_h

#include "structure.h"

struct stPhysicalObject {
    readonly pointer visualset;
    readonly pointer physical_collideset;
    readonly pointer visual_bounding_volume;
    readonly pointer collide_bounding_volume;
} typedef tdstPhysicalObject, tdstPO;

#endif /* stPhysicalObject_h */
