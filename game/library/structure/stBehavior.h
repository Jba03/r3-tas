#ifndef stBehavior_h
#define stBehavior_h

#include "structure.h"

struct stBehavior {
    readonly char8 name[256]; /* 256 on GCN, at least */
    readonly pointer scripts;
    readonly pointer script_first;
    readonly uint8 n_scripts;
    padding(3)
} typedef tdstBehavior, tdstComport;

struct stScriptAI {
    readonly pointer behavior;
    readonly uint32 n_behaviors;
    readonly uint32 no_initialization;
    readonly uint8 n_acttable_entries;
    padding(3)
} typedef tdstScriptAI;

#endif /* stBehavior_h */
