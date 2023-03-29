#ifndef stEngineTimer_h
#define stEngineTimer_h

#include "structure.h"

struct stEngineTimer {
    readonly uint32 frame;
    readonly int16 timerHandle;
    padding(2)
    readonly uint32 currentCount;
    readonly uint32 deltaCount;
    readonly uint32 counter[16];
    readonly uint32 usefulDeltaTime;
    readonly uint32 pauseTime;
    readonly float32 frameLength;
    readonly struct { readonly uint32_t low, high; } totalRealTime;
    readonly struct { readonly uint32_t low, high; } totalPauseTime;
    readonly uint32 ticksPerMs;
} typedef tdstEngineTimer;

#endif /* stEngineTimer_h */
