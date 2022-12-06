//
//  engine_timer.h
//  r3tools
//
//  Created by Jba03 on 2022-11-12.
//

#ifndef engine_timer_h
#define engine_timer_h

#include "stream.h"

#define ENGINE_TIMER

struct EngineTimerCount {
    long low, high;
};

struct EngineTimer {
    uint32_t frame;
    address timer_handle;
    uint32_t timer_count_current;
    uint32_t timer_count_delta;
    uint32_t counter[16];
    uint32_t delta_time_useful;
    uint32_t pause_time;
    float framelength;    
    struct EngineTimerCount time_real, time_pause;
    uint32_t ticks_per_ms;
    
    address offset;
};

ENGINE_TIMER struct EngineTimer engine_timer_read(struct Stream *stream);
    
#endif /* engine_timer_h */
