#ifndef sector_h
#define sector_h

#include "structure.h"
#include "list.h"

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
    
    readonly struct vector3 aabb_min;
    readonly struct vector3 aabb_max;
    
    readonly float far_plane;
    readonly uint8 is_virtual;
    readonly int8 camera_type;
    readonly int8 counter;
    readonly int8 priority;
    readonly pointer skymaterial;
    readonly uint8 fog;
    
#if platform == GCN
    readonly const char name[0x104];
#endif
};

#define sector_is_virtual(sector) sector->is_virtual
#define sector_camera_type(sector) sector->camera_type
#define sector_get_counter(sector) sector->counter
#define sector_priority(sector) sector->priority

#endif /* sector_h */
