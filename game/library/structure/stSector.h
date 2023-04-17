#ifndef stSector_h
#define stSector_h

#include "structure.h"
#include "stLinkedList.h"
#include "stVector3D.h"

#define sector_priority_min     0
#define sector_priority_normal  64
#define sector_priority_max     127

typedef struct stSector tdstSector;

struct stSector
{
    readonly tdstDoublyLinkedList characterList;
    readonly tdstDoublyLinkedList staticLightList;
    readonly tdstDoublyLinkedList dynamicLightList;
    readonly tdstDoublyLinkedList graphicSectorList;
    readonly tdstDoublyLinkedList collisionSectorList;
    readonly tdstDoublyLinkedList activitySectorList;
    readonly tdstDoublyLinkedList soundSectorList;
    readonly tdstDoublyLinkedList soundEventList;
    readonly tdstVector3D min;
    readonly tdstVector3D max;
    readonly float32 farPlane;
    readonly uint8 isVirtual;
    readonly int8 cameraType;
    readonly int8 counter;
    readonly int8 priority;
    readonly pointer skyMaterial;
    readonly uint8 fog;
#if platform == GCN
    readonly char8 name[0x104];
#endif
};

#define sector_is_virtual(sector) sector->isVirtual
#define sector_camera_type(sector) sector->cameraType
#define sector_get_counter(sector) sector->counter
#define sector_priority(sector) sector->priority

#ifdef USE_FUNCTIONS

/** fnSectorGetName: get the abbreviated name of a sector */
const char* fnSectorGetName(const tdstSector* sct);

#endif
#endif /* stSector_h */
