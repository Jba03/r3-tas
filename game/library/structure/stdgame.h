#ifndef stdgame_h
#define stdgame_h

#include "structure.h"

struct standard_game_info
{
    readonly int32 family_type;
    readonly int32 model_type;
    readonly int32 instance_type;
    readonly pointer superobject;
    readonly uint8 initial_flag;
    readonly uint8 flag1;
    readonly uint8 flag2;
    
    padding(2)
    
    readonly uint32 last_frame;
    readonly uint32 capabilities;
    readonly uint8 traction_factor;
    readonly uint8 hitpoints;
    readonly uint8 hitpoints_max;
    readonly uint8 hitpoints_max_max;
    readonly uint32 custom_bits;
    readonly uint32 ai_custom_bits;
    readonly uint8 platform_type;
    readonly uint8 misc_flags;
    readonly uint8 transparency_zone_min;
    readonly uint8 transparency_zone_max;
    readonly uint32 custom_bits_initial;
    readonly uint32 ai_custom_bits_initial;
    readonly uint8 hitpoints_initial;
    readonly uint8 hitpoints_max_initial;
    readonly uint8 misc_flags_initial;
    readonly uint8 too_far_limit;
    readonly uint8 importance;
    readonly uint8 optional;
    
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
