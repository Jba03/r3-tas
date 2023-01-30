#ifndef ipo_h
#define ipo_h

#include "structure.h"

struct ipo {
    readonly pointer physical_object;
    readonly pointer current_radiosity;
    readonly doublepointer radiosity;
    readonly pointer portal_camera;
    readonly uint32 last_transition_id;
    readonly float32 last_ratio_used;
#if platform == GCN || platform == PC
    padding(4) /* Unknown? */
    readonly char8 name[0x32];
#endif
};

#endif /* ipo_h */
