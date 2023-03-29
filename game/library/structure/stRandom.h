#ifndef stRandom_h
#define stRandom_h

#include "structure.h"

struct stRandom {
    readonly uint32 table_size;
    readonly uint32 table_indices[50];
    readonly uint32 last_index;
    readonly uint32 table_max;
    readonly float32 table_max_inverse;
    readonly pointer table;
} typedef tdstRandom;

#if USE_FUNCTIONS

/** fnRNDTableIndex: index an RNG table. */
int32 fnRNDTableIndex(const tdstRandom *rnd, unsigned index, int offset);

/** fnRNDCall: call the random number device `n_calls` times. */
int32 fnRNDCall(const tdstRandom *rnd, unsigned n_calls, unsigned index, unsigned mi, unsigned ma);

#endif
#endif /* stRandom_h */
