#ifndef stBrain_h
#define stBrain_h

#include "structure.h"

struct stBrain {
    readonly pointer mind;
    readonly pointer lastNoCollideMaterial;
    readonly uint8 warnMechanics;
    readonly uint8 activeDuringTransition;
    padding(2)
} typedef tdstBrain;

#endif /* stBrain_h */
