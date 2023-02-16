#ifndef engine_timer_h
#define engine_timer_h

#include "structure.h"

struct engine_timer
{
    readonly uint32 frame;
    readonly int16 timer_handle;
    padding(2)
    readonly uint32 timer_count_current;
    readonly uint32 timer_count_delta;
    readonly uint32 counter[16];
    readonly uint32 delta_time_useful;
    readonly uint32 pause_time;
    readonly float32 frame_length;
    readonly struct { readonly uint32_t low, high; } time_real;
    readonly struct { readonly uint32_t low, high; } time_pause;
    readonly uint32 ticks_per_ms;
};

#endif /* engine_timer_h */
