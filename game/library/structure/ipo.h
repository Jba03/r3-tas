#ifndef ipo_h
#define ipo_h

#include "structure.h"

struct ipo
{
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

#if USE_FUNCTIONS

#include "collide_object.h"

/** ipo_name: get the name of an IPO */
const char* ipo_name(const struct ipo* ipo);

/** ipo_collide_object: get the collide object (ZDR) of an IPO */
const struct collide_object* ipo_collide_object(const struct ipo* ipo);

#endif
#endif /* ipo_h */
