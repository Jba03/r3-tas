#ifndef sector_h
#define sector_h

#include "structure.h"
#include "list.h"
#include "vector3.h"

#define sector_priority_min     0
#define sector_priority_normal  64
#define sector_priority_max     127

struct sector
{
    readonly doubly_linked_list characters;
    readonly doubly_linked_list static_lights;
    readonly doubly_linked_list dynamic_lights;
    readonly doubly_linked_list graphic_sectors;
    readonly doubly_linked_list collision_sectors;
    readonly doubly_linked_list activity_sectors;
    readonly doubly_linked_list sound_sectors;
    readonly doubly_linked_list sound_events;
    readonly struct vector3 min;
    readonly struct vector3 max;
    readonly float32 far_plane;
    readonly uint8 is_virtual;
    readonly int8 camera_type;
    readonly int8 counter;
    readonly int8 priority;
    readonly pointer skymaterial;
    readonly uint8 fog;
#if platform == GCN
    readonly char8 name[0x104];
#endif
};

#define sector_is_virtual(sector) sector->is_virtual
#define sector_camera_type(sector) sector->camera_type
#define sector_get_counter(sector) sector->counter
#define sector_priority(sector) sector->priority

#if USE_FUNCTIONS

/** sector_name: get the abbreviated name of a sector */
const char* sector_name(const struct sector* sct);

#endif
#endif /* sector_h */
