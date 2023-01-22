//
//  collideset.h
//  r3-tas-memorymap
//
//  Created by Jba03 on 2023-01-09.
//

#ifndef collideset_h
#define collideset_h

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
    readonly float collider_real[2];
    readonly uint8_t collider_type;
    readonly uint8_t collider_prio;
    readonly uint8_t unused[2];
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
    readonly uint32_t zdd_privileged_zone;
    readonly uint32_t zde_privileged_zone;
    readonly uint32_t zdm_privileged_zone;
    readonly uint32_t zdr_privileged_zone;
    readonly uint8_t computefrequency;
    readonly uint8_t character_priority;
    readonly uint8_t collision_flag;
    padding(1)
    readonly struct collider_info collider_info;
};

struct zdx_list
{
    readonly pointer first_element;
    readonly pointer unknown; /* gc only? mirror of above field */
    readonly int32_t n_elements;
    readonly uint16_t n_zdx;
    padding(2)
};

#endif /* collideset_h */
