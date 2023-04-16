#ifndef stBehavior_h
#define stBehavior_h

#include "structure.h"

typedef struct stBehavior tdstBehavior, tdstComport;
typedef struct stScriptAI tdstScriptAI;

struct stBehavior
{
    readonly char8 name[256]; /* 256 on GCN, at least */
    readonly pointer scripts;
    readonly pointer firstScript;
    readonly uint8 numScripts;
    readonly padding(3)
};

struct stScriptAI
{
    readonly pointer behavior;
    readonly uint32 numBehaviors;
    readonly uint32 noInitialization;
    readonly uint8 numActionTableEntries;
    readonly padding(3)
};

#endif /* stBehavior_h */
