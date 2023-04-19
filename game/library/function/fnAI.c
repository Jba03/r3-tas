//
//  aifunction.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-22.
//

#include <stdio.h>

#include "game.h"
#include "memory.h"
#include "stEngineStructure.h"



/** ai_function_temporal_real_combination: interpolation function based on engine frame time */
const float ai_function_temporal_real_combination(const float a, const float coefficient, const float b)
{
    /* Get engine delta time */
    const uint32 dt = host_byteorder_32(engine->timer.usefulDeltaTime);
    /* Get length of the frame */
    const float framelength = host_byteorder_f32(*(float32*)&engine->timer.frameLength);
    /* Calculate exponent  */
    const int power = 0.5f + (float)dt / (1000.0f * framelength);
    
    return powf(coefficient, power <= 5 ? power : 6) * (a - b) + b;
}

/** ai_function_get_time: get current timer count  */
const uint32 ai_function_get_time(void)
{
    return host_byteorder_32(engine->timer.currentCount);
}

/** ai_function_get_time: get time elapsed since `v` */
const uint32 ai_function_get_elapsed_time(const uint32 v)
{
    uint32 current = host_byteorder_32(engine->timer.currentCount);
    return (current >= v) ? (current - v) : (uint32)(-(int32)(current - v));
}
