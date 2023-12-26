#ifndef stMind_h
#define stMind_h

#include "library.hh"
#include "stIntelligence.hh"
#include "stDsg.hh"

typedef struct stMind stMind;

struct stMind
{
    readonly pointer<> aiModel;
    readonly pointer<stIntelligence> intelligence;
    readonly pointer<> reflex;
    readonly pointer<stDsgMem> dsgMem;
    readonly uint8 runIntelligence;
    readonly padding(3)
};

#endif /* stMind_h */
