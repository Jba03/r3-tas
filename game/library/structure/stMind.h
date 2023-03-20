#ifndef stMind_h
#define stMind_h

#include "structure.h"

struct stMind {
    readonly pointer ai_model;
    readonly pointer intelligence;
    readonly pointer reflex;
    readonly pointer dsgmemory;
    readonly uint8 run_intelligence;
    padding(3)
} typedef tdstMind;

#endif /* stMind_h */
