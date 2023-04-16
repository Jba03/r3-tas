#ifndef stBrain_h
#define stBrain_h

#include "structure.h"

typedef struct stBrain tdstBrain;

struct stBrain
{
    readonly pointer mind;
    readonly pointer lastNoCollideMaterial;
    readonly uint8 warnMechanics;
    readonly uint8 activeDuringTransition;
    padding(2)
};

#endif /* stBrain_h */
