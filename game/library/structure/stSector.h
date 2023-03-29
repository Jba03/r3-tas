#ifndef stSector_h
#define stSector_h

#include "structure.h"
#include "stLinkedList.h"
#include "stVector3D.h"

#define sector_priority_min     0
#define sector_priority_normal  64
#define sector_priority_max     127

struct stSector {
    readonly tdstDoublyLinkedList characters;
    readonly tdstDoublyLinkedList static_lights;
    readonly tdstDoublyLinkedList dynamic_lights;
    readonly tdstDoublyLinkedList graphic_sectors;
    readonly tdstDoublyLinkedList collision_sectors;
    readonly tdstDoublyLinkedList activity_sectors;
    readonly tdstDoublyLinkedList sound_sectors;
    readonly tdstDoublyLinkedList sound_events;
    readonly tdstVector3D min;
    readonly tdstVector3D max;
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
} typedef tdstSector;

#define sector_is_virtual(sector) sector->is_virtual
#define sector_camera_type(sector) sector->camera_type
#define sector_get_counter(sector) sector->counter
#define sector_priority(sector) sector->priority

#if USE_FUNCTIONS

/** fnSectorGetName: get the abbreviated name of a sector */
const char* fnSectorGetName(const tdstSector* sct);

#endif
#endif /* stSector_h */
