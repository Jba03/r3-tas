//
//  bruteforce.h
//  r3-tas
//
//  Created by Jba03 on 2023-06-26.
//

#ifndef bruteforce_h
#define bruteforce_h

#include <map>

//#include "stDynamics.hh"
#include "emulator.hh"
//#include "stMatrix4D.hh"
//#include "stDynamics.hh"

#include <stdbool.h>

struct State
{
    struct GCPadStatus gamepad;
    struct stDynamics dynamics;
    union stMatrix4D globalMatrix;
    unsigned int frame;
    float score;
};

struct BruteforcingTool
{
    bool (*cycle)(void);
    bool isEnabled;
};

struct state {
  stVector3D proposedSpeed;
  stVector3D currentTranslation; /* to become stTransform */
  uint32_t dynamicsFlags;
};

struct attempt {
  int index;
  int frameCount = 0;
  bool optimal = false;
  
  std::vector<state> states;
};

struct block {
  uint64_t seed;
  state state;
};

struct bruteforcingTool {
  bruteforcingTool();
  
  std::vector<block> blocks;
};



extern attempt *optimalAttempt;
extern std::vector<attempt> attempts;

extern std::map<uint64_t, attempt*> blockMap;




//struct bState {
//  // The iteration this block was reached
//  uint64_t iterationReached = 0;
//  
//  // Dynamics state
//  //stDynamics dynamics;
//  // The translation to load when the block is selected
//  stVector3D absolutePosition = stVector3D(0.0f, 0.0f, 0.0f);
//  // The speed proposed (towards the next block)
//  stVector3D proposeSpeed = stVector3D(0.0f, 0.0f, 0.0f);
//  // The next block, which the proposed speed reaches
//  // nullptr if this block hasn't yet been processed
//  bState *next = nullptr;
//  
//  uint32 flags = 0;
//  uint32 endFlags = 0;
//  stVector3D previousSpeed = stVector3D();
//  
//  bState() {}
//  bState(stDynamics *dyn) {
//    absolutePosition = dyn->base.currentTransform.position();
//    flags = dyn->base.flags;
//    endFlags = dyn->base.endFlags;
//    //dynamics = *dyn;
//  }
//  
//  // Load state into specified superobject
//  auto load(stSuperObject *spo) -> void {
//    stEngineObject *object = spo->data;
//    stDynam *dynam = object->dynam;
//    stDynamics *dyn = dynam->dynamics;
//    
//    //dyn->base.flags = flags;
//    //dyn->base.endFlags = endFlags;
//    //dyn->base.gravity = game_byteorder_f32(9.82f);
//    //dyn->base.proposeSpeed = dynamics.base.proposeSpeed;
//    //dyn->base.previousSpeed = dynamics.base.previousSpeed;
//    
//    dyn->base.currentTransform.setTranslation(absolutePosition);
//    dyn->base.previousTransform.setTranslation(absolutePosition);
////    *(stTransform*)spo->globalTransform = dyn->base.currentTransform;
////    dyn->base.previousSpeed = previousSpeed;
//  }
//};
//
//extern std::map<uint64_t, bState> blocks;
//
//
//




#endif /* bruteforce_h */
