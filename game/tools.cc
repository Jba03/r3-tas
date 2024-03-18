#include "tools.hh"
#include "game.hh"

namespace R3 {
  
#pragma mark - AutoSplitter
  
  AutoSplitter::AutoSplitter() {
    connect();
  }
  
  void AutoSplitter::setLoadTimesRemoved(bool removeLoadTimes) {
    loadRemovedSplits = removeLoadTimes;
  }
  
  void AutoSplitter::setSplitTrigger(SplitTrigger trigger) {
    whenToSplit = trigger;
  }
  
  void AutoSplitter::split() {
    printf("split!\n");
  }
  
  bool AutoSplitter::connect() {
    return false;
  }
  
  void AutoSplitter::update() {
    if (game::engineModeChangedTo(ChangeLevel)) {
      if (whenToSplit == EndTriggerTouch) {
        split();
      }
    } else if (game::engineModeChangedTo(EnterLevel, /* from */ ChangeLevel)) {
      if (whenToSplit == StarsAppear) {
        split();
      } else if (whenToSplit == FirstStar) {
        splitDelay = 27;
      }
    }
    
    if (splitDelay-- > 0) {
      if (splitDelay == 0) split();
    }
  }
  
  AutoSplitter autoSplitter;
  
};
