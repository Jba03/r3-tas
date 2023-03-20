#ifndef stCollideSet_h
#define stCollideSet_h

#include "stLinkedList.h"

struct stPhysicalCollideSet {
    readonly pointer zdm;
    readonly pointer zdd;
    readonly pointer zde;
    readonly pointer zdr;
} typedef tdstPhysicalCollideSet;

struct stColliderInfo {
    readonly pointer collider_actors[2]; /* ::so */
    readonly tdstVector3D collider_vectors[2];
    readonly float32 collider_real[2];
    readonly uint8 collider_type;
    readonly uint8 collider_prio;
    readonly uint8 unused[2];
} typedef tdstColliderInfo;

struct stCollideSet {
    readonly pointer zdd_list;
    readonly pointer zde_list;
    readonly pointer zdm_list;
    readonly pointer zdr_list;
    readonly pointer zdd_activation_list;
    readonly pointer zde_activation_list;
    readonly pointer zdm_activation_list;
    readonly pointer zdr_activation_list;
    readonly pointer zdd_activation_current;
    readonly pointer zde_activation_current;
    readonly pointer zdr_activation_current;
    readonly pointer zdm_activation_current;
    readonly uint32 zdd_privileged_zone;
    readonly uint32 zde_privileged_zone;
    readonly uint32 zdm_privileged_zone;
    readonly uint32 zdr_privileged_zone;
    readonly uint8 computefrequency;
    readonly uint8 character_priority;
    readonly uint8 collision_flag;
    padding(1)
    readonly tdstColliderInfo collider_info;
} typedef tdstCollideSet;

struct stZdxList {
    readonly tdstDoublyLinkedList list;
    readonly uint16 n_zdx;
    padding(2)
} typedef tdstZdxList;

#endif /* stCollideSet_h */
