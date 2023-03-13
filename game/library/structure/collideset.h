#ifndef collideset_h
#define collideset_h

#include "list.h"

struct physical_collideset
{
    readonly pointer zdm;
    readonly pointer zdd;
    readonly pointer zde;
    readonly pointer zdr;
};

struct collider_info
{
    readonly pointer collider_actors[2]; /* ::so */
    readonly struct vector3 collider_vectors[2];
    readonly float32 collider_real[2];
    readonly uint8 collider_type;
    readonly uint8 collider_prio;
    readonly uint8 unused[2];
};

struct actor_collideset
{
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
    readonly struct collider_info collider_info;
};

struct zdx_list
{
    readonly doubly_linked_list list;
    readonly uint16 n_zdx;
    padding(2)
};

#endif /* collideset_h */
