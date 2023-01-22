#ifndef engine_timer_h
#define engine_timer_h

#include "structure.h"

struct engine_timer
{
    readonly uint32_t frame;
    readonly uint16_t timer_handle;
    padding(2)
    readonly uint32_t timer_count_current;
    readonly uint32_t timer_count_delta;
    readonly uint32_t counter[16];
    readonly uint32_t delta_time_useful;
    readonly uint32_t pause_time;
    readonly float frame_length;
    padding(4)
    readonly struct { readonly uint64_t low, high; } time_real;
    readonly struct { readonly uint64_t low, high; } time_pause;
    readonly uint32_t ticks_per_ms;
    padding(4)
};

#endif /* engine_timer_h */
