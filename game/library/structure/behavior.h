#ifndef behavior_h
#define behavior_h

#include "structure.h"

struct behavior
{
    readonly char8 name[256]; /* 256 on GCN, at least */
    readonly pointer scripts;
    readonly pointer script_first;
    readonly uint8 n_scripts;
    padding(3)
};

struct behavior_list
{
    readonly pointer behavior;
    readonly uint32 n_behaviors;
    readonly uint32 no_initialization;
    readonly uint8 n_acttable_entries;
    padding(3)
};

#endif /* behavior_h */
