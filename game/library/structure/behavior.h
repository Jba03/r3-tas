#ifndef behavior_h
#define behavior_h

#include "structure.h"

struct behavior
{
    readonly const char name[256]; /* 256 on GCN, at least */
    readonly pointer scripts;
    readonly pointer script_first;
    readonly uint8_t n_scripts;
    padding(3)
};

struct behavior_list
{
    readonly pointer behavior;
    readonly uint32_t n_behaviors;
    readonly uint32_t no_initialization;
    readonly uint8_t n_acttable_entries;
    padding(3)
};

#endif /* behavior_h */
