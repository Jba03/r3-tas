#ifndef stEngineTimer_h
#define stEngineTimer_h

#include "library.hh"

struct stEngineTimer {
  readonly uint32 frame;
  readonly int16 timerHandle;
  readonly padding(2)
  readonly uint32 currentCount;
  readonly uint32 deltaCount;
  readonly uint32 counter[16];
  readonly uint32 usefulDeltaTime;
  readonly uint32 pauseTime;
  readonly float32 frameLength;
  readonly uint32 totalRealTimeLow;
  readonly uint32 totalRealTimeHigh;
  readonly uint32 totalPauseTimeLow;
  readonly uint32 totalPauseTimeHigh;
  readonly uint32 ticksPerMs;
};

#endif /* stEngineTimer_h */
