//
//  engine_timer.c
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#include "engine_timer.h"

ENGINE_TIMER struct EngineTimer engine_timer_read(struct Stream *stream)
{
    struct EngineTimer timer;
    timer.offset = stream->position;
    
    timer.frame = read32();
    timer.timer_handle = read32();
    timer.timer_count_current = read32();
    timer.timer_count_delta = read32();
    
    for (int i = 0; i < 16; i++)
        timer.counter[i] = read32();
    
    timer.delta_time_useful = read32();
    timer.pause_time = read32();
    timer.framelength = readfloat();
    
    timer.time_real = read32() << 31 | read32();
    timer.time_pause = read32() << 31 | read32();
    
    timer.ticks_per_ms = read32();
    
    return timer;
}
