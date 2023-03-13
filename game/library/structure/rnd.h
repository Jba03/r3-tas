#ifndef rnd_h
#define rnd_h

#include "structure.h"

struct rnd
{
    readonly uint32 table_size;
    readonly uint32 table_indices[50];
    readonly uint32 last_index;
    readonly uint32 table_max;
    readonly float32 table_max_inverse;
    readonly pointer table;
};

#if USE_FUNCTIONS

/** rnd_table_index: index an RNG table. */
int32 rnd_table_index(const struct rnd *rnd, unsigned index, int offset);

/** rnd_call: call the random number device `n_calls` times. */
int32 rnd_call(const struct rnd *rnd, unsigned n_calls, unsigned index, unsigned mi, unsigned ma);

#endif
#endif /* rnd_h */
