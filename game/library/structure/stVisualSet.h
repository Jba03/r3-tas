#ifndef stVisualSet_h
#define stVisualSet_h

#include "structure.h"

struct stVisualSet {
    readonly float32 lastDistance;
    readonly int16 numLod;
    readonly int16 type;
    readonly pointer thresholdTable;
    readonly pointer lodDefinitions;
    readonly doublepointer RLI;
    readonly int32 numRLI;
} typedef tdstVisualSet;

union stVisualObject {
    readonly pointer geometricObject;
    readonly pointer morphObject;
} typedef tdstVisualObject;

#endif /* stVisualSet_h */
