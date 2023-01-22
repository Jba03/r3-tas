#ifndef brain_h
#define brain_h

#include "structure.h"

struct brain
{
    readonly pointer mind;
    readonly pointer last_nocollide_material;
    readonly uint8_t warn_mechanics;
    readonly uint8_t active_during_transition;
    padding(2)
};

#endif /* brain_h */
