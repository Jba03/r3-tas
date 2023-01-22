#ifndef input_h
#define input_h

#include "structure.h"

struct input_entry
{
    padding(6 * 4) /* ? */
    readonly uint32_t num_keywords;
    readonly pointer keyword_array;
    readonly pointer action_name;
    readonly pointer entry_name;
    readonly int32_t state;
    readonly float analogvalue;
    readonly int8_t active;
    padding(3)
};

#endif /* input_h */
