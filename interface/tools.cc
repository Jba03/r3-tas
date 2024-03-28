#include "tools.hh"
#include "game.hh"
#include "interface.hh"

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
    
    event("EngineModeChanged").subscribe("AutoSplitter", [&](Event::Param& p) {
      eEngineMode from = std::any_cast<eEngineMode>(p["from"]);
      eEngineMode to = std::any_cast<eEngineMode>(p["to"]);
      if (from == ChangeLevel && whenToSplit == EndTriggerTouch) {
        split();
      } else if (from == ChangeLevel && to == EnterLevel) {
        if (whenToSplit == StarsAppear) split();
        if (whenToSplit == FirstStar) splitDelay = 27;
      }
    });
    
    return false;
  }
  
  void AutoSplitter::update() {
//    if (game::engineModeChangedTo(ChangeLevel)) {
//      if (whenToSplit == EndTriggerTouch) {
//        split();
//      }
//    } else if (game::engineModeChangedTo(EnterLevel, /* from */ ChangeLevel)) {
//      if (whenToSplit == StarsAppear) {
//        split();
//      } else if (whenToSplit == FirstStar) {
//        splitDelay = 27;
//      }
//    }
    
    if (splitDelay-- > 0) {
      if (splitDelay == 0) split();
    }
  }
  
  AutoSplitter autoSplitter;
  
#pragma mark - AI Debugger
  
};
