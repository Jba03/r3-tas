#ifndef stRandom_h
#define stRandom_h

#include "structure.h"

typedef struct stRandom tdstRandom;

struct stRandom
{
    readonly uint32 tableSize;
    readonly uint32 tableIndices[50];
    readonly uint32 lastIndex;
    readonly uint32 tableMax;
    readonly float32 tableMaxInverse;
    readonly pointer table;
};

#ifdef USE_FUNCTIONS

/** fnRNDTableIndex: index an RNG table. */
int32 fnRNDTableIndex(const tdstRandom *rnd, unsigned index, int offset);

/** fnRNDCall: call the random number device `n_calls` times. */
int32 fnRNDCall(const tdstRandom *rnd, unsigned n_calls, unsigned index, unsigned mi, unsigned ma);

#endif
#endif /* stRandom_h */
