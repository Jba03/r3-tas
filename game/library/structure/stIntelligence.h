#ifndef stIntelligence_h
#define stIntelligence_h

#include "structure.h"

struct stIntelligence {
    readonly doublepointer scriptAI;
    readonly pointer currentTree;
    readonly pointer currentBehavior;
    readonly pointer previousBehavior;
    readonly pointer actionTable;
    readonly uint32 initializeBehavior;
} typedef tdstIntelligence;

#endif /* stIntelligence_h */
