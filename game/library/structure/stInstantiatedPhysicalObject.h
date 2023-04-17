#ifndef stInstantiatedPhysicalObject_h
#define stInstantiatedPhysicalObject_h

#include "structure.h"

typedef struct stInstantiatedPhysicalObject tdstInstantiatedPhysicalObject, tdstIPO;

struct stInstantiatedPhysicalObject
{
    readonly pointer tdstPhysicalObject;
    readonly pointer currentRadiosity;
    readonly doublepointer radiosity;
    readonly pointer portalCamera;
    readonly uint32 lastTransitionID;
    readonly float32 lastRatioUsed;
#if platform == GCN || platform == PC
    readonly padding(4) /* Unknown? */
    readonly char8 name[0x32];
#endif
};

#ifdef USE_FUNCTIONS

#include "stSuperobject.h"
#include "stCollideObject.h"

/** fnIPOGetName: get the name of an IPO */
const char* fnIPOGetName(const tdstInstantiatedPhysicalObject* ipo);

/* ipo_world_matrix: compute the world transform of an IPO */
const tdstMatrix4D ipo_world_matrix(const tdstSuperObject* ipo);

/** fnIPOGetCollideObject: get the collide object (ZDR) of an IPO */
const tdstCollideObject* fnIPOGetCollideObject(const tdstInstantiatedPhysicalObject* ipo);

tdstInstantiatedPhysicalObject* fnFindIPO(const char* name, tdstSuperObject *root, tdstSuperObject** so);

#endif
#endif /* stInstantiatedPhysicalObject_h */
