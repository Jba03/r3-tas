#ifndef stBehavior_h
#define stBehavior_h

#include "structure.h"

struct stBehavior {
    readonly char8 name[256]; /* 256 on GCN, at least */
    readonly pointer scripts;
    readonly pointer firstScript;
    readonly uint8 numScripts;
    padding(3)
} typedef tdstBehavior, tdstComport;

struct stScriptAI {
    readonly pointer behavior;
    readonly uint32 numBehaviors;
    readonly uint32 noInitialization;
    readonly uint8 numActionTableEntries;
    padding(3)
} typedef tdstScriptAI;

#endif /* stBehavior_h */
