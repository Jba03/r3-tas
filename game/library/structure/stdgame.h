#ifndef stdgame_h
#define stdgame_h

#include "structure.h"

struct standard_game_info
{
    readonly int32_t family_type;
    readonly int32_t model_type;
    readonly int32_t instance_type;
    readonly pointer superobject;
    readonly uint8_t initial_flag;
    readonly uint8_t flag1;
    readonly uint8_t flag2;
    
    padding(2)
    
    readonly uint32_t last_frame;
    readonly uint32_t capabilities;
    readonly uint8_t traction_factor;
    readonly uint8_t hitpoints;
    readonly uint8_t hitpoints_max;
    readonly uint8_t hitpoints_max_max;
    readonly uint32_t custom_bits;
    readonly uint32_t ai_custom_bits;
    readonly uint8_t platform_type;
    readonly uint8_t misc_flags;
    readonly uint8_t transparency_zone_min;
    readonly uint8_t transparency_zone_max;
    readonly uint32_t custom_bits_initial;
    readonly uint32_t ai_custom_bits_initial;
    readonly uint8_t hitpoints_initial;
    readonly uint8_t hitpoints_max_initial;
    readonly uint8_t misc_flags_initial;
    readonly uint8_t too_far_limit;
    readonly uint8_t importance;
    readonly uint8_t optional;
    
    padding(2)
    
    /* :: custom values :: */
};

/**
 * stdgame_get_superobject:
 *  Get the superobject whose actor holds this info.
 */
#define stdgame_get_superobject(stdgame) ((struct superobject*)pointer(stdgame->superobject))

#define stdgame_family_type(stdgame) host_byteorder_32(stdgame->family_type)
#define stdgame_model_type(stdgame) host_byteorder_32(stdgame->model_type)
#define stdgame_instance_type(stdgame) host_byteorder_32(stdgame->instance_type)

#endif /* stdgame_h */
