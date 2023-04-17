//
//  input.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-20.
//

#include "memory.h"
#include "stInputStructure.h"
#include "game.h"

tdstInputEntryElement* fnInputEntryFind(const tdstInputStructure* s, const char* action_name)
{
    tdstInputEntryElement* entry = pointer(s->entries);
    int c = host_byteorder_32(s->numEntries);
    while (--c && entry)
    {
        const char* name = pointer(entry->actionName);
        if (strcmp(name, action_name) == 0) return entry;
        entry++;
    }
    
    return NULL;
}
