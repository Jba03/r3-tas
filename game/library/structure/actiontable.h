#ifndef actiontable_h
#define actiontable_h

#include "structure.h"

struct action_entry
{
    readonly char8 name[0x50];
    readonly uint32 param[8];
    readonly pointer tree;
    readonly uint8 used;
    readonly uint8 n_rules;
    readonly uint8 default_return;
    readonly uint8 new_return;
};

struct actiontable
{
    readonly pointer entries;
    readonly uint8 n_entries;
    readonly uint8 n_entries_used;
    readonly uint8 entry_current;
    padding(1)
};

#endif /* actiontable_h */
