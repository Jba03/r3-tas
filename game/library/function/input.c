//
//  input.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-20.
//

#include "input.h"
#include "game.h"

struct input_entry* input_entry_find(const struct input_structure* s, const char* action_name)
{
    struct input_entry* entry = pointer(s->entries);
    int c = host_byteorder_32(s->n_entries);
    while (--c && entry)
    {
        const char* name = pointer(entry->action_name);
        if (strcmp(name, action_name) == 0) return entry;
        entry++;
    }
    
    return NULL;
}
