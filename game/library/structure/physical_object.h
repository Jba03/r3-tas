#ifndef physical_object_h
#define physical_object_h

#include "structure.h"

struct physical_object {
    readonly pointer visualset;
    readonly pointer physical_collideset;
    readonly pointer visual_bounding_volume;
    readonly pointer collide_bounding_volume;
};

#endif /* physical_object_h */
