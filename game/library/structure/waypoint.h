#ifndef waypoint_h
#define waypoint_h

#include "structure.h"

struct waypoint
{
    readonly vector3 position;
    readonly float radius;
    readonly pointer superobject;
};

#endif /* waypoint_h */
