#ifndef stVisualSet_h
#define stVisualSet_h

#include "library.hh"

typedef struct stVisualSet stVisualSet;
typedef union uVisualObject tduVisualObject, tduVSObject;

struct stVisualSet
{
    readonly float32 lastDistance;
    readonly int16 numLod;
    readonly int16 type;
    readonly pointer<> thresholdTable;
    readonly pointer<> lodDefinitions;
    readonly doublepointer<> RLI;
    readonly int32 numRLI;
};

union uVisualObject
{
    readonly pointer<> geometricObject;
    readonly pointer<> morphObject;
};

#endif /* stVisualSet_h */
