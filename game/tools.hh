#ifndef tools_hh
#define tools_hh

#include <chrono>

class HelperTool {
  void run();
};

namespace R3 {
  
  struct AutoSplitter : HelperTool {
    enum SplitTrigger {
      EndTriggerTouch, // When end trigger touched
      StarsAppear, // Frame stars first appear
      FirstStar, // Frame first star punched in
      LevelChange, // Frame of level change
    };
    AutoSplitter();
    void setLoadTimesRemoved(bool removeLoadTimes);
    void setSplitTrigger(SplitTrigger trigger);
    void update();
  private:
    SplitTrigger whenToSplit = FirstStar;
    // Remove time spent loading the game?
    bool loadRemovedSplits;
    // Default total time, in ms
    double totalTime;
    // Load-removed total time, in ms
    double totalTimeLRT;
    // Delay until split
    int splitDelay = 0;
    
    bool connect();
    void split();
  };
  
  extern AutoSplitter autoSplitter;
  
};

#endif /* tools_hh */
