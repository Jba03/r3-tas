#ifndef stMind_h
#define stMind_h

#include "structure.h"

typedef struct stMind tdstMind;

struct stMind
{
    readonly pointer aiModel;
    readonly pointer intelligence;
    readonly pointer reflex;
    readonly pointer dsgMem;
    readonly uint8 runIntelligence;
    readonly padding(3)
};

#endif /* stMind_h */
