#ifndef stActionTable_h
#define stActionTable_h

#include "structure.h"

struct stActionTableEntry {
    readonly char8 name[0x50];
    readonly uint32 param[8];
    padding(4) /* ? */
    padding(4) /* ? */
    readonly pointer name_pointer; /* ? */
    readonly pointer tree;
    readonly uint8 used;
    readonly uint8 n_rules;
    readonly uint8 use_default_return;
    readonly uint8 new_return;
} typedef tdstActionTableEntry;

struct stActionTable {
    readonly pointer entries;
    readonly uint8 n_entries;
    readonly uint8 n_entries_used;
    readonly uint8 entry_current;
    padding(1)
} typedef tdstActionTable;

#endif /* stActionTable_h */
