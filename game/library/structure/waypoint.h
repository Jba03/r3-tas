#ifndef waypoint_h
#define waypoint_h

#include "structure.h"

struct waypoint
{
    readonly struct vector3 position;
    readonly float32 radius;
    readonly pointer superobject;
};

#endif /* waypoint_h */
