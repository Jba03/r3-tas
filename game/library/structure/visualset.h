#ifndef visualset_h
#define visualset_h

#include "structure.h"

struct visualset
{
    readonly float32 last_distance;
    readonly int16 n_lod;
    readonly int16 type;
    readonly pointer threshold_table;
    readonly pointer lod_definitions;
    readonly doublepointer rli;
    readonly int32 n_rli;
};

union visualobject
{
    readonly pointer geometric_object;
    readonly pointer morph_object;
};

#endif /* visualset_h */
