#ifndef stBrain_h
#define stBrain_h

#include "library.hh"
#include "stMind.hh"

typedef struct stBrain stBrain;

struct stBrain
{
    readonly pointer<stMind> mind;
    readonly pointer<> lastNoCollideMaterial;
    readonly uint8 warnMechanics;
    readonly uint8 activeDuringTransition;
    readonly padding(2)
};

#endif /* stBrain_h */
