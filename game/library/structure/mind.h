#ifndef mind_h
#define mind_h

#include "structure.h"

struct mind
{
    readonly pointer ai_model;
    readonly pointer intelligence;
    readonly pointer reflex;
    readonly pointer dsgmemory;
    readonly uint8_t run_intelligence;
    padding(3)
};

#endif /* mind_h */
