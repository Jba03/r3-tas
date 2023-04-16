#ifndef stGraph_h
#define stGraph_h

#include "structure.h"

typedef struct stGraph tdstGraph;

struct stGraph
{
    readonly pointer firstNode;
    readonly pointer lastNode;
    readonly int32 numNodes;
};

#endif /* stGraph_h */
