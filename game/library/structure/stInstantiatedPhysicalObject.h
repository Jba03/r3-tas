#ifndef stInstantiatedPhysicalObject_h
#define stInstantiatedPhysicalObject_h

#include "structure.h"

struct stInstantiatedPhysicalObject {
    readonly pointer tdstPhysicalObject;
    readonly pointer current_radiosity;
    readonly doublepointer radiosity;
    readonly pointer portal_camera;
    readonly uint32 last_transition_id;
    readonly float32 last_ratio_used;
#if platform == GCN || platform == PC
    padding(4) /* Unknown? */
    readonly char8 name[0x32];
#endif
} typedef tdstInstantiatedPhysicalObject, tdstIPO;

#if USE_FUNCTIONS

#include "stCollideObject.h"

/** ipo_name: get the name of an IPO */
const char* ipo_name(const tdstInstantiatedPhysicalObject* ipo);

/** ipo_collide_object: get the collide object (ZDR) of an IPO */
const tdstCollideObject* ipo_collide_object(const tdstInstantiatedPhysicalObject* ipo);

#endif
#endif /* stInstantiatedPhysicalObject_h */
