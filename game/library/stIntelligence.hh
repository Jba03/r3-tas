#ifndef stIntelligence_h
#define stIntelligence_h

#include "library.hh"
#include "stBehavior.hh"

typedef struct stIntelligence stIntelligence;

struct stIntelligence
{
    readonly doublepointer<> scriptAI;
    readonly pointer<> currentTree;
    readonly pointer<stBehavior> currentBehavior;
    readonly pointer<stBehavior> previousBehavior;
    readonly pointer<> actionTable;
    readonly uint32 initializeBehavior;
};

#endif /* stIntelligence_h */
