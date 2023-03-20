#ifndef stGraphNode_h
#define stGraphNode_h

#include "structure.h"

struct stGraphNode {
    readonly pointer next;
    readonly pointer prev;
    readonly pointer parent;
    readonly pointer waypoint;
    readonly int32 type;
    readonly int32 type_initial;
    readonly pointer arc_list;
} typedef tdstGraphNode;

#endif /* stGraphNode_h */
