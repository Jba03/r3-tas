#ifndef stActionTable_h
#define stActionTable_h

#include "structure.h"

typedef struct stActionTableEntry tdstActionTableEntry;
typedef struct stActionTable tdstActionTable;

struct stActionTableEntry
{
    readonly char8 name[0x50];
    readonly uint32 param[8];
    readonly padding(4) /* ? */
    readonly padding(4) /* ? */
    readonly pointer namePointer; /* ? */
    readonly pointer tree;
    readonly uint8 used;
    readonly uint8 numRules;
    readonly uint8 useDefaultReturn;
    readonly uint8 newReturn;
};

struct stActionTable
{
    readonly pointer entries;
    readonly uint8 numEntries;
    readonly uint8 numEntriesUsed;
    readonly uint8 currentEntry;
    readonly padding(1)
};

#endif /* stActionTable_h */
