#ifndef stMind_h
#define stMind_h

#include "structure.h"

struct stMind {
    readonly pointer aiModel;
    readonly pointer intelligence;
    readonly pointer reflex;
    readonly pointer dsgMem;
    readonly uint8 runIntelligence;
    padding(3)
} typedef tdstMind;

#endif /* stMind_h */
