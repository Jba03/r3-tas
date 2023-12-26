#ifndef stWayPoint_h
#define stWayPoint_h

#include "library.hh"

typedef struct stWayPoint stWayPoint;

struct stWayPoint
{
    readonly stVector3D position;
    readonly float32 radius;
    readonly pointer<> superObject;
};

#endif /* stWayPoint_h */
