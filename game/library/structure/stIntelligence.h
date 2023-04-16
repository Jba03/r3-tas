#ifndef stIntelligence_h
#define stIntelligence_h

#include "structure.h"

typedef struct stIntelligence tdstIntelligence;

struct stIntelligence
{
    readonly doublepointer scriptAI;
    readonly pointer currentTree;
    readonly pointer currentBehavior;
    readonly pointer previousBehavior;
    readonly pointer actionTable;
    readonly uint32 initializeBehavior;
};

#endif /* stIntelligence_h */
