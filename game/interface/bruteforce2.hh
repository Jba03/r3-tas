//
//  bruteforce.h
//  r3-tas
//
//  Created by Jba03 on 2023-06-26.
//

#ifndef bruteforce_h
#define bruteforce_h

//#include "stDynamics.hh"
#include "emulator.hh"
#include "structure.hh"
#include <map>

#include <stdbool.h>

#include "robin_hood.h"

struct bruteforcingTool {
  bruteforcingTool();
};

#define bInputActionJump  (1 << 0)
#define bInputActionHover (1 << 1)

struct bInputState {
  float mJoyX;
  float mJoyY;
  float cJoyX;
  float cJoyY;
  uint8_t action;
  
  bInputState() {}
};

struct bIntelligenceState {
  pointer<stBehavior> currentBehavior = 0;
  pointer<stBehavior> previousBehavior = 0;
  uint8_t *buffer = nullptr;
  
  auto load(stSuperObject *spo) -> void {
    stEngineObject *obj = (stEngineObject*)spo->data;
    stBrain *brain = obj->brain;
    stMind *mind = brain->mind;
    stIntelligence *intel = mind->intelligence;
    stDsgMem *mem = mind->dsgMem;
    
    intel->currentBehavior = currentBehavior;
    intel->previousBehavior = previousBehavior;
    
    memcpy(mem->currentBuffer, buffer, 5000);
  }
  
  bIntelligenceState() {}
  bIntelligenceState(stSuperObject *spo) {
    stEngineObject *obj = (stEngineObject*)spo->data;
    stBrain *brain = obj->brain;
    stMind *mind = brain->mind;
    stIntelligence *intel = mind->intelligence;
    stDsgMem *mem = mind->dsgMem;
    
    currentBehavior = intel->currentBehavior;
    previousBehavior = intel->previousBehavior;
    
    buffer = (uint8_t*)malloc(5000);
    memcpy(buffer, mem->currentBuffer, 5000);
  }
};

struct bPhysicsState {
  // Dynamics state
  stDynamics dynamics = {0};
  // Dynamics report
  stDynamicsReport dynamicsReport = {0};
  // Transform
  stTransform transform;
  // Initialized
  bool initialized = false;
  
  bPhysicsState() {}
};

struct bState {
  // The tree depth of this block
  uint32_t depth = 0;
  // The previous block in the sequence
  // which happened to reach this block.
  bState *prev = nullptr;
  // The next block in the sequence, which is reached
  // when the dynamics state for this block is loaded.
  bState *next = nullptr;
  
  // State
  bIntelligenceState intelligenceState;
  bPhysicsState physicsState;
  bInputState inputState;
  
  int timesLoaded = 0;
  
  auto setIntelligenceState(bIntelligenceState s) -> void {
    intelligenceState = s;
  }
  
  auto setPhysicsState(bPhysicsState s) -> void {
    physicsState = s;
  }
  
  auto setInputState(bInputState s) -> void {
    inputState = s;
  }
  
  auto setTransform(stTransform *T) -> void {
    std::memcpy(&physicsState.transform, T, sizeof *T);
  }
  
  // Set the dynamics state for this block
  auto setDynamics(stDynamics *dynamics) -> void {
   // physicsState.dynamics = (stDynamics*)malloc(sizeof(stDynamics));
    //physicsState.dynamicsReport = (stDynamicsReport*)malloc(sizeof(stDynamicsReport));
    
    std::memcpy(&physicsState.dynamics, dynamics, sizeof *dynamics);
    std::memcpy(&physicsState.dynamicsReport, (void*)dynamics->base.report, sizeof(stDynamicsReport));
  }
  
  // Load the block state into specified actor
  auto load(stSuperObject *spo, stDynamics *dyn) -> void {
    stEngineObject *obj = (stEngineObject*)spo->data;
    
    //intelligenceState.load(spo);
    
    //uint32_t idCard = dyn->base.idcard.pointeeAddress();

    //if (physicsState.dynamics)
    std::memcpy(dyn, &physicsState.dynamics, sizeof *dyn);
    
    //printf("dyn: %p\n", physicsState.dynamics);
    
    //std::memcpy(&dyn->base.currentTransform, &physicsState.dynamics->base.currentTransform, sizeof(stTransform));
    //std::memcpy(&dyn->base.previousTransform, &physicsState.dynamics->base.previousTransform, sizeof(stTransform));
    std::memcpy(spo->globalTransform, &physicsState.transform, sizeof(stTransform));

    //*(uint32_t*)&dyn->base.idcard = game_byteorder_32(idCard);

    std::memcpy((void*)dyn->base.report, &physicsState.dynamics.base.report, sizeof(stDynamicsReport));
    //*(stDynamicsReport*)dyn->base.report = *(stDynamicsReport*)physicsState.dynamics->base.report;
    
    timesLoaded++;
  }
  
  bState() {}
};

extern robin_hood::unordered_map<uint64_t, bState> blocks;

#endif /* bruteforce_h */
