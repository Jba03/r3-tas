#ifndef graphnode_h
#define graphnode_h

#include "structure.h"

struct graphnode
{
    readonly pointer next;
    readonly pointer prev;
    readonly pointer parent;
    readonly pointer waypoint;
    readonly int32_t type;
    readonly int32_t type_initial;
    readonly pointer arc_list;
};

#endif /* graphnode_h */
