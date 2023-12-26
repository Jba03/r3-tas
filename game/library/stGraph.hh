#ifndef stGraph_h
#define stGraph_h

#include "library.hh"

typedef struct stGraph stGraph;

struct stGraph
{
    readonly pointer<> firstNode;
    readonly pointer<> lastNode;
    readonly int32 numNodes;
};

#endif /* stGraph_h */
