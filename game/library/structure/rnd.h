#ifndef rnd_h
#define rnd_h

#include "structure.h"

struct rnd
{
    readonly uint32_t table_size;
    readonly uint32_t table_indices[50];
    readonly uint32_t last_index;
    readonly uint32_t table_max;
    readonly uint32_t table_max_inverse; /* float */
    readonly pointer table;
};

#endif /* rnd_h */
