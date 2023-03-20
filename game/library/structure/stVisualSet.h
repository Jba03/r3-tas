#ifndef stVisualSet_h
#define stVisualSet_h

#include "structure.h"

struct stVisualSet {
    readonly float32 last_distance;
    readonly int16 n_lod;
    readonly int16 type;
    readonly pointer threshold_table;
    readonly pointer lod_definitions;
    readonly doublepointer rli;
    readonly int32 n_rli;
} typedef tdstVisualSet;

union stVisualObject {
    readonly pointer geometric_object;
    readonly pointer morph_object;
} typedef tdstVisualObject;

#endif /* stVisualSet_h */
