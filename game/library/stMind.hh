#ifndef stMind_h
#define stMind_h

#include "library.hh"

typedef struct stMind stMind;

struct stMind
{
    readonly pointer<> aiModel;
    readonly pointer<> intelligence;
    readonly pointer<> reflex;
    readonly pointer<> dsgMem;
    readonly uint8 runIntelligence;
    readonly padding(3)
};

#endif /* stMind_h */
