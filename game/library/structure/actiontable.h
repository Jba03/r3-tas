#ifndef actiontable_h
#define actiontable_h

#include "structure.h"

// 0x50
//
//

struct action_entry
{
    readonly char name[0x50];
    readonly uint32_t param[8];
    readonly pointer tree;
    readonly uint8_t used;
    readonly uint8_t n_rules;
    readonly uint8_t default_return;
    readonly uint8_t new_return;
};

struct actiontable
{
    readonly pointer entries;
    readonly uint8_t n_entries;
    readonly uint8_t n_entries_used;
    readonly uint8_t entry_current;
    padding(1)
};

#endif /* actiontable_h */
