#ifndef stGraphNode_h
#define stGraphNode_h

#include "structure.h"

struct stGraphNode {
    readonly pointer next;
    readonly pointer prev;
    readonly pointer parent;
    readonly pointer wayPoint;
    readonly int32 type;
    readonly int32 initialType;
    readonly pointer arcList;
} typedef tdstGraphNode;

#endif /* stGraphNode_h */
