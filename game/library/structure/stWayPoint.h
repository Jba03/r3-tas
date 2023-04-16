#ifndef stWayPoint_h
#define stWayPoint_h

#include "structure.h"

typedef struct stWayPoint tdstWayPoint;

struct stWayPoint
{
    readonly tdstVector3D position;
    readonly float32 radius;
    readonly pointer superObject;
};

#endif /* stWayPoint_h */
