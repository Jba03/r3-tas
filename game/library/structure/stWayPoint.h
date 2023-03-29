#ifndef stWayPoint_h
#define stWayPoint_h

#include "structure.h"

struct stWayPoint {
    readonly tdstVector3D position;
    readonly float32 radius;
    readonly pointer superObject;
} typedef tdstWayPoint;

#endif /* stWayPoint_h */
