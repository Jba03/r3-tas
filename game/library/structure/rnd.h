#ifndef rnd_h
#define rnd_h

#include "structure.h"

struct rnd
{
    readonly uint32 table_size;
    readonly uint32 table_indices[50];
    readonly uint32 last_index;
    readonly uint32 table_max;
    readonly uint32 table_max_inverse; /* float */
    readonly pointer table;
};

#endif /* rnd_h */
