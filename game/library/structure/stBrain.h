#ifndef stBrain_h
#define stBrain_h

#include "structure.h"

struct stBrain {
    readonly pointer mind;
    readonly pointer last_nocollide_material;
    readonly uint8 warn_mechanics;
    readonly uint8 active_during_transition;
    padding(2)
} typedef tdstBrain;

#endif /* stBrain_h */
