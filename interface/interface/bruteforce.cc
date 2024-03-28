//
//  bruteforce.c
//  r3-tas
//
//  Created by Jba03 on 2023-06-26.
//

#include "bruteforce.hh"
#include "game.hh"
#include "structure.hh"
#include "hook.hh"

#include <map>
#include <random>

static const std::mt19937 rnd;

static unsigned engineLoopCount = 50;
static unsigned engineLoopCurrent = 0;
static bool skipTiming = true;

static unsigned iteration = 0;
static unsigned maxIterations = 10;


static unsigned parity = 0;
static unsigned traa = 0;

static void input()
{
    //game::g_stEngineStructure->inputMode = 1;
    //game::IPT_g_stInputStructure->entries[15].state = rand()%3;
    for (int i = 0; i < 226; i++)
    {
        //printf("%s\n", (char*)game::IPT_g_stInputStructure->entries[i].actionName);
    }
 
    //printf("input\n");
    
//    if (traa++ < 10)
//    {
//        game::IPT_g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeY].analogValue = game_byteorder_f32(-128.0f);
//    }
//    else
//    {
//        game::IPT_g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeY].analogValue = game_byteorder_f32(-0.0f);
//    }
    
    //game::IPT_g_stInputStructure->entries[IPT_E_Entry_Action_Sauter].state = game_byteorder_32(rand()%3);
    //game::IPT_g_stInputStructure->entries[IPT_E_Entry_Action_Helico].state = game_byteorder_32(rand()%3);
    
//    if ((parity++ % 10) == 0 && parity < 226*10)
//    {
//        printf("trying %s (%d)\n", (char*)game::IPT_g_stInputStructure->entries[traa].actionName, traa);
//        game::IPT_g_stInputStructure->entries[traa].state = game_byteorder_32(1);
//        traa++;
//    }
    
    //printf("%X\n", game::IPT_g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeY].state.offset());
}

static void jumpToTimingFunction()
{
    std::memset(emulator::ppcState.gpr, 0, sizeof(uint32_t) * 32);
    GPR(0)  = 0x80035d14;
    GPR(1)  = 0x803c86f8;
    GPR(2)  = 0x805e19e0;
    GPR(3)  = 0x00000001;
    GPR(4)  = 0xc049ef4f;
    GPR(5)  = 0x80475c52;
    GPR(6)  = 0x803c855f;
    GPR(7)  = 0x41a56ab0;
    GPR(8)  = 0xc2a2b355;
    GPR(9)  = 0x803c8470;
    GPR(10) = 0x80f3676c;
    GPR(11) = 0x02000000;
    GPR(12) = 0x803dc4f4;
    GPR(13) = 0x805def80;
    GPR(30) = 0x803e7c0c;
    LR  = 0x80035d4c;
    NPC = 0x80034ca0;
}

static void jumpToEngine()
{
    std::memset(emulator::ppcState.gpr, 0, sizeof(uint32_t) * 32);
    GPR(1)  = 0x803c86f8;
    GPR(2)  = 0x805e19e0;
    GPR(4)  = 0x803e0000;
    GPR(5)  = 0x0000b032;
    GPR(9)  = 0x00000003;
    GPR(10) = 0x80225f80;
    GPR(11) = 0x00000020;
    GPR(12) = 0x80037078;
    GPR(13) = 0x805def80;
    GPR(30) = 0x803e7c0c;
    LR = 0x80035cb8;
    NPC = 0x80037078;
}

static void timingFunctionStart()
{
    
    
    //if (skipTiming) NPC = LR;
}

static void timingFunctionEnd()
{
    //printf("end of timing\n");
    
    uint32_t sp = GPR(1) & 0x7FFFFFFF;
    GPR(31) = host_byteorder_32(*(uint32_t*)(memory::baseAddress + sp + 0x1C));
    GPR(0) = host_byteorder_32(*(uint32_t*)(memory::baseAddress + sp + 0x24));
    GPR(1) += 0x20;
    NPC = LR = GPR(0);
    
    if (++iteration < maxIterations) jumpToEngine();
}

static void engineStart()
{
    input();
    
    //printf("engine\n");
    //emulator::pushProcessorState();
}

static void engineEnd()
{
    uint32_t sp = GPR(1) & 0x7FFFFFFF; // sp = r1
    GPR(29) = host_byteorder_32(*(uint32_t*)(memory::baseAddress + sp + 0xC4));
    GPR(30) = host_byteorder_32(*(uint32_t*)(memory::baseAddress + sp + 0xC8));
    GPR(31) = host_byteorder_32(*(uint32_t*)(memory::baseAddress + sp + 0xCC));
    GPR(0)  = host_byteorder_32(*(uint32_t*)(memory::baseAddress + sp + 0xD4));
    GPR(1) += 0xD0; // align sp
    LR = GPR(0);
    NPC = LR; // jump
    
    if (iteration < maxIterations)
        jumpToTimingFunction();
    else
        iteration = 0;
//
//    if (++engineLoopCurrent < engineLoopCount)
//    {
//        skipTiming = false;
//        //emulator::popProcessorState();
//
//        //skipTiming = false;
//
//
//        /* Engine finished executing - push return state and jump to timing */
//        //emulator::pushProcessorState();
//        //jumpToTimingFunction();
//
//        /* Check so timing isn't executed twice the last iteration */
//        //if (engineLoopCurrent != engineLoopCount - 1)
//        {
//            jumpToTimingFunction();
//        }
//
//
//
//        /* At this point, the engine won't execute, as we have not yet made a time step. */
//        /* Trick the game into thinking the frame has already been drawn. */
//        //game::g_stEngineStructure->timer.frame = host_byteorder_32((uint32_t)game::g_stEngineStructure->timer.frame + 1);
//        /* On average, the cycle count is 650000 per frame. */
//        //if ((uint64_t)game::g_stEngineStructure->timer.totalRealTimeLow + 650000 > 0xFFFFFFFF)
//            //game::g_stEngineStructure->timer.totalRealTimeHigh = host_byteorder_32((uint32_t)game::g_stEngineStructure->timer.totalRealTimeHigh + 1);
//
//        //game::g_stEngineStructure->timer.totalRealTimeLow = host_byteorder_32((uint32_t)game::g_stEngineStructure->timer.totalRealTimeLow + 650000);
//    }
//    else
//    {
//        engineLoopCurrent = 0;
//        /* Skip next call to timing function */
//        skipTiming = true;
//    }
}

static void frameStart()
{
    emulator::pushProcessorState();
    printf("frame start\n");
}

static void frameEnd()
{
    uint32_t sp = GPR(1) & 0x7FFFFFFF; /* sp = r1 */
    GPR(28) = host_byteorder_32(*(uint32_t*)(memory::baseAddress + sp + 0x10));
    GPR(29) = host_byteorder_32(*(uint32_t*)(memory::baseAddress + sp + 0x14));
    GPR(30) = host_byteorder_32(*(uint32_t*)(memory::baseAddress + sp + 0x18));
    GPR(31) = host_byteorder_32(*(uint32_t*)(memory::baseAddress + sp + 0x1C));
    GPR(0)  = host_byteorder_32(*(uint32_t*)(memory::baseAddress + sp + 0x24)); /* lr = r0 */
    GPR(1) += 32; /* SP alignment */
    NPC = LR = GPR(0); // jump
    
    if (++engineLoopCurrent < engineLoopCount)
    {
        std::memset(emulator::ppcState.gpr, 0, sizeof(uint32_t) * 32);
        
        //emulator::popProcessorState();
        
        //printf("loop\n");
        GPR(0)  = 0x80035edc;
        GPR(1)  = 0x803c86f8;
        GPR(2)  = 0x805e19e0;
        GPR(3)  = 0x00000001;
        GPR(4)  = 0xcc010000;
        GPR(5)  = 0x80272bc8;
        GPR(6)  = 0x410054ad;
        GPR(7)  = 0x40000017;
        GPR(8)  = 0x8081ada4;
        GPR(9)  = 0x00000002;
        GPR(10) = 0x00000156;
        GPR(11) = 0x00000020;
        GPR(12) = 0x8007326c;
        GPR(13) = 0x805def80;
        GPR(29) = 0x00000001;
        GPR(30) = 0x803e7c0c;

        LR = 0x80035edc;
        NPC = 0x80035bd4;
    }
    else
    {
        printf("\n");
        engineLoopCurrent = 0;
    }
}

static uint32_t savedTime;

static inline float rround(float v, float precision = 1.0f) {
  return std::round(v / precision) * precision;
}

static const uint64_t getRoughPositionHash(stVector3D pos) {
  float x = rround(pos.x, 0.1f);
  float y = rround(pos.y, 0.1f);
  float z = rround(pos.z, 0.1f);
  
  uint64_t qx = std::hash<float>()(x);
  uint64_t qy = std::hash<float>()(y);
  uint64_t qz = std::hash<float>()(z);
  
  return (qx ^ (qy << 1)) ^ qz; //(qx << 32) | qy | (qz << 24);
}

static void setPosition(stSuperObject *spo, stVector3D T) {
  stEngineObject *engineObject = spo->data;
  stDynam *dynam = engineObject->dynam;
  stDynamics *dynamics = dynam->dynamics;
  
  dynamics->base.currentTransform.setTranslation(T);
  dynamics->base.previousTransform.setTranslation(T);
  *(stTransform*)spo->globalTransform = dynamics->base.currentTransform;
  
  if (dynamics->base.endFlags & dynamics_size_complex) {
    dynamics->complex.previousMatrixAbsolute.setTranslation(T);
    dynamics->complex.previousMatrixPrevious.setTranslation(T);
  }
}

static void proposeSpeed(stDynamics *dyn, stVector3D speed) {
//  dyn->base.flags |= dynamicsFlagSpeedProposeX;
//  dyn->base.flags |= dynamicsFlagSpeedProposeY;
//  //dyn->base.flags |= dynamicsFlagSpeedProposeZ;
//  dyn->base.flags |= dynamicsFlagSpeedProposeAbsolute;
//  dyn->base.proposeSpeed = speed.game();
}

static void imposeSpeedZ(stDynamics *dyn, float speed) {
//  dyn->base.flags |= dynamicsFlagSpeedImposeZ;
//  //dyn->base.flags |=~dynamicsFlagSpeedImposeAbsolute;
//  *(uint32_t*)&dyn->base.imposeSpeed.z = game_byteorder_f32(speed);
}

static void setMechanicInertiaXYZ(stDynamics *dyn, stVector3D inertia) {
  *(uint32_t*)&dyn->advanced.xInertia = game_byteorder_f32(inertia.x);
  *(uint32_t*)&dyn->advanced.yInertia = game_byteorder_f32(inertia.y);
  *(uint32_t*)&dyn->advanced.zInertia = game_byteorder_f32(inertia.z);
}

static float jumpSpeed(int stage) {
  switch (stage) {
    case 1: return 12.0f;
    case 2: return 12.0f;
    case 3: 
    default: return 0.0f;
  }
}

static void jumpBehavior(stDynamics *dyn) {
  setMechanicInertiaXYZ(dyn, stVector3D(0.0f, 0.0f, 1.0f));
  imposeSpeedZ(dyn, 12.0f);
}

static void loadState(stSuperObject *spo, state s) {
  setPosition(spo, s.currentTranslation);
  
  stEngineObject *engineObject = spo->data;
  stDynam *dynam = engineObject->dynam;
  stDynamics *dynamics = dynam->dynamics;
  dynamics->base.flags = game_byteorder_32(s.dynamicsFlags);
  
}

std::vector<attempt> attempts;
static int currentAttemptIndex = 0;
static attempt *currentAttempt = nullptr;
static stVector3D currentProposeSpeed;

static const stVector3D target = stVector3D(-34.87, 11.69, -5.00);
//stVector3D(-17.91, 21.59, -4.96);
//stVector3D(-86.23, 19.94, -7.92);
//stVector3D(63.79, 21.01, -2.82);
static const uint64_t targetHash = getRoughPositionHash(target);
static float baseDistance = INFINITY;

static bool tryEraseAttempt(attempt& a) {
  if (a.states.size() == 0) return false;
  stVector3D current = a.states.back().currentTranslation;
  //current.x = 0;
  float l = (current - target).length();
  if (l > 20.0f) {
    attempts.erase(attempts.begin() + a.index);
    return true;
  }
  return false;
}

static bool finished = false;

std::map<uint64_t, attempt*> blockMap;
attempt *optimalAttempt = nullptr;

static bool firstRun = true;

static int attemptIndex = 0;

static float averageLength = 0;

static void updateDynamics(stSuperObject *obj) {
  stEngineObject *engineObject = obj->data;
  stDynam *dynam = engineObject->dynam;
  stDynamics *dynamics = dynam->dynamics;
  
  if (finished) return;
  
  if (blockMap.size() > 0) {
    
    attempt *attempt = nullptr;
    while (attempt != nullptr) {
      auto it = blockMap.begin();
      std::advance(it, rand() % blockMap.size());
      attempt = blockMap[it->first];
      
      if ((attempt->states.back().currentTranslation - target).length() > baseDistance)
        attempt = nullptr;
      
      if (attempt->frameCount < averageLength && rand()%2==0)
        attempt = nullptr;
    }
    
    if (attempt) {
      if (attempt->states.size() > 0) {
        loadState(obj, attempt->states.back());
      }
     // currentAttempt = attempt;
      currentAttempt = new struct attempt();
      currentAttempt->frameCount = attempt->frameCount;
    }
    
    // Begin with an angle opposite to the target
    float angle = 180.0f;
    //while (fabs(angle) > 130.0f)
    {
      currentProposeSpeed.x = 14.0f * (drand48() * 2.0f - 1.0f);
      currentProposeSpeed.y = 14.0f * (drand48() * 2.0f - 1.0f);
      currentProposeSpeed.z = 0.0f;
      
      const float sA = atan2(currentProposeSpeed.y, currentProposeSpeed.x);
      
      const stVector3D pA = dynamics->base.currentTransform.position();
      const stVector3D pB = target;
      
      const float hR = atan2(float(pB.y) - float(pA.y), float(pB.x) - float(pA.x));
      // Then, calculate the new, relative angle of both
      angle = degrees(hR - sA);
    }
    
  } else {
    // Initial move
    currentProposeSpeed.x = 14.0f * (drand48() * 2.0f - 1.0f);
    currentProposeSpeed.y = 14.0f * (drand48() * 2.0f - 1.0f);
    currentProposeSpeed.z = 0.0f;
    
    currentAttempt = new struct attempt();
    blockMap[getRoughPositionHash(dynamics->base.currentTransform.position())] = currentAttempt;
    
    baseDistance = (dynamics->base.currentTransform.position() - target).length();
  }
  
  if (engineLoopCurrent % engineLoopCount == 0) {
    printf("blocks: %d, average: %f\n", blockMap.size(), averageLength);
  }
  
  /* Select a random attempt */
  //currentAttemptIndex = std::rand() % attempts.size();
  //attempt& a = attempts[currentAttemptIndex];
  
  //if (tryEraseAttempt(a)) return;
  
  //printf("furthering attempt %d\n", currentAttemptIndex);
  
//  if (a.states.size() > 0) {
//    loadState(obj, a.states.back());
//  }
  
  
  proposeSpeed(dynamics, currentProposeSpeed);
  
  
  //jumpBehavior(dynamics);
  
  // MECBase.c: MEC_vInitTranslation
}

static void saveCalculatedState(stSuperObject *spo) {
  if (finished) return;
  
  stEngineObject *engineObject = spo->data;
  stDynam *dynam = engineObject->dynam;
  stDynamics *dynamics = dynam->dynamics;
  
  state s;
  s.proposedSpeed = currentProposeSpeed;
  //state.currentTranslation = dynamics->base.currentTransform.position();
  s.currentTranslation = dynamics->base.previousTransform.position();
  s.dynamicsFlags = dynamics->base.flags;
  
  if (!currentAttempt) return;
  
  currentAttempt->frameCount++;
  currentAttempt->states.push_back(s);
  
  // Get hash of this position
  uint64_t hash = getRoughPositionHash(s.currentTranslation);
  // Try to find a valid attempt for the block at this position
  if (blockMap.find(hash) != blockMap.end()) {
    attempt* existingAttempt = blockMap.at(hash);
    if (existingAttempt) {
      printf("existing attempt @ %X: %p (%d vs %d)\n", hash, existingAttempt, existingAttempt->frameCount, currentAttempt->frameCount);
      if (currentAttempt->frameCount < existingAttempt->frameCount) {
        // The new attempt is faster, update the block.
       // printf("updating attempt %d\n", existingAttempt->index);
        //blockMap.at(hash) = currentAttempt;
        existingAttempt->states.back() = s;
      }
    }
  } else {
    // No existing attempt was found, so insert a copy of new one.
    attempt *clone = new struct attempt();
    *clone = *currentAttempt;
    clone->states = std::vector<state>(clone->states);
    clone->index = attemptIndex++;
    currentAttempt = clone;
    
    averageLength += clone->frameCount;
    averageLength /= blockMap.size();
    
    blockMap[hash] = currentAttempt;
  }
  
  if (getRoughPositionHash(s.currentTranslation) == targetHash) {
    optimalAttempt = currentAttempt;
    finished = true;
    engineLoopCount = 1;
  }
}

enum functionHookLocation { S, E };

template <functionHookLocation at>
static void fn_vMakeCharacterThink() {
  if (at == S) NPC = LR;
  // We need to trick the game into running intelligence at
  // the loop speed factor, while running mechanics at normal speed.
//  uint32_t step = 1000.0f / (60.0f / float(at == S ? engineLoopCount : 1));
//  if (engineLoopCurrent % engineLoopCount == 0) step -= engineLoopCount;
//
//  printf("step: (%s) %d\n", at == S ? "start" : "end", step);
//
//  game::g_stEngineStructure->timer.deltaCount = game_byteorder_32(step);
//  game::g_stEngineStructure->timer.usefulDeltaTime = game_byteorder_32(step);
}



static bool firstIteration = true;
std::map<uint64_t, bState> blocks;
std::vector<bState*> heuristic;
static bState *currentBlock = nullptr;
static bState *initialBlock = nullptr;
static float averageNumIterations = 0.0f;
static float selectionBias = 1.0f;
static stVector3D startPosition;

// Called before physics are processed
static auto bStep(stSuperObject *spo) -> void {
  
  if (finished) return;
  
  stEngineObject *object = spo->data;
  stDynam *dynam = object->dynam;
  stDynamics *dynamics = dynam->dynamics;
  
  if (firstIteration) {
    // Insert the block for the initial position
    bState state(dynamics);
    uint64_t hash = getRoughPositionHash(state.absolutePosition);
    blocks[hash] = state;
    
    // Initialize some global variables
    initialBlock = &blocks[hash];
    startPosition = state.absolutePosition;
    baseDistance = (state.absolutePosition - target).length();
    
    firstIteration = false;
  }
  
  // Select a random block from the map.
  bState *block = nullptr;
  while (block == nullptr) {
    auto it = blocks.begin();
    std::advance(it, rand() % blocks.size());
    block = &blocks[it->first];

//    if ((block->absolutePosition - target).length() > baseDistance) {
//      block = nullptr;
//    }
    
    if (block->iterationReached < averageNumIterations - selectionBias) {
      blocks.erase(it);
      block = nullptr;
    }
  }
  
  block->load(spo);
  
  // Load the block
  //setPosition(spo, block->absolutePosition);

  // To retain the block in bStepDone()
  currentBlock = block;

  stVector3D speed = stVector3D(0.0f, 0.0f, 0.0f);
  
  float angle = 180.0f;
  // Small bias towards the angle
  while (fabs(angle) > 20.0f)
  {
    speed.x = 14.0f * (drand48() * 2.0f - 1.0f);
    speed.y = 14.0f * (drand48() * 2.0f - 1.0f);
    
    const float sA = atan2(speed.y, speed.x);
    
    const stVector3D pA = dynamics->base.currentTransform.position();
    const stVector3D pB = target;
    
    const float hR = atan2(pB.y - pA.y, pB.x - pA.x);
    // Then, calculate the new, relative angle of both
    angle = degrees(hR - sA);
  }
  
  
//  if (iteration % maxIterations == 0) {
//    // Purge some blocks.
//    // Find the block which contains the most optimal route.
//    long shortestIteration = INFINITY;
//    bState *selected = nullptr;
//    for (std::pair<uint64_t, bState> pair : blocks) {
//      bState *search = &pair.second;
//      // Find the last blocks in the tree.
//      if (search->iterationReached < shortestIteration && search->next == nullptr) {
//        shortestIteration = search->iterationReached;
//        selected = search;
//      }
//    }
//
//    auto it = blocks.begin();
//    for (int i = 0; i < blocks.size(); i++) {
//      std::advance(it, i);
//      if (blocks.) blocks.erase(it);
//    }
//
//    printf("shortest: %d\n", shortestIteration);
//
//  }
  
  block->proposeSpeed = speed;

  proposeSpeed(dynamics, speed);
}

// Called when physics finished processing
static auto bStepDone(stSuperObject *spo) -> void {
  
  if (finished) return;
  
  stEngineObject *object = spo->data;
  stDynam *dynam = object->dynam;
  stDynamics *dynamics = dynam->dynamics;
  
  // Get the current position hash
  uint64_t hash = getRoughPositionHash(dynamics->base.currentTransform.position());
  
  // Check if there is already a block at this position
  if (blocks.find(hash) != blocks.end()) {
    bState *block = &blocks.at(hash);
    // There is a block for this position.
    // Was it reached faster than the previous time?
    if (currentBlock->iterationReached + 1 < block->iterationReached) {
      // If so, tell the block it was reached faster.
      block->iterationReached = currentBlock->iterationReached + 1;
      // And set the next node of the current block
      currentBlock->next = block;
    }
  } else {
    ///printf("create a block for %llx\n", hash);
    // There is no block for this position, so we need to create it.
    bState state(dynamics);
    state.iterationReached = currentBlock->iterationReached + 1;
    state.previousSpeed = dynamics->base.previousSpeed;
    
    blocks[hash] = state;
    
    //printf("iteration reached: %d\n", state.iterationReached);
    averageNumIterations += state.iterationReached;
    averageNumIterations /= 2.0f;
    
    averageNumIterations = 2;
  }
  
  if (getRoughPositionHash(dynamics->base.currentTransform.position()) == targetHash) {
    finished = true;
    engineLoopCount = 1;
  }
}

static bState *cur_block;
static int iterationsPerBlock = 100;
static int currentBlockIteration = 0;

static auto bStep2(stSuperObject *spo) -> void {
  
  stEngineObject *object = spo->data;
  stDynam *dynam = object->dynam;
  stDynamics *dynamics = dynam->dynamics;
  
  if (firstIteration) {
    
    bState state(dynamics);
    state.iterationReached = 0;
    
    uint64_t hash = getRoughPositionHash(dynamics->base.currentTransform.position());
    blocks[hash] = state;
    cur_block = &blocks[hash];
    
    firstIteration = false;
  }
  
  cur_block->load(spo);
  
  
  stVector3D speed = dynamics->base.previousSpeed.host();
  
  float angle = 180.0f;
  // Small bias towards the angle
  while (fabs(angle) > 20.0f)
  {
    speed.x = 14.0f * (drand48() * 2.0f - 1.0f);
    speed.y = 14.0f * (drand48() * 2.0f - 1.0f);
    
    const float sA = atan2(speed.y, speed.x);
    
    const stVector3D pA = dynamics->base.currentTransform.position();
    const stVector3D pB = target;
    
    const float hR = atan2(pB.y - pA.y, pB.x - pA.x);
    // Then, calculate the new, relative angle of both
    angle = degrees(hR - sA);
  }
  
  
  proposeSpeed(dynamics, speed);
  
}

static auto bStepDone2(stSuperObject *spo) -> void {
  
  stEngineObject *object = spo->data;
  stDynam *dynam = object->dynam;
  stDynamics *dynamics = dynam->dynamics;
  
  uint64_t hash = getRoughPositionHash(dynamics->base.currentTransform.position());
  
  // Check if there is already a block at this position
  if (blocks.find(hash) != blocks.end()) {
    bState *block = &blocks.at(hash);
    // There is a block for this position.
    // Was it reached faster than the previous time?
    if ((dynamics->base.currentTransform.position() - target).length() < (block->absolutePosition - target).length()) {
      // If so, tell the block it was reached faster.
      //block->iterationReached = currentBlock->iterationReached + 1;
      //cur_b
      block->absolutePosition = dynamics->base.currentTransform.position();
      // And set the next node of the current block
      cur_block->next = block;
      
      printf("change to %f from %f\n", (dynamics->base.currentTransform.position() - target).length(), (block->absolutePosition - target).length());
    }
  } else {
    ///printf("create a block for %llx\n", hash);
    // There is no block for this position, so we need to create it.
    bState state(dynamics);
    state.iterationReached = currentBlock->iterationReached + 1;
    state.previousSpeed = dynamics->base.previousSpeed;
    
    blocks[hash] = state;
  }
  
  
  if ((++currentBlockIteration % iterationsPerBlock) == 0) {
    cur_block = &blocks[hash];
  }
  
}

static clock_t clocktt = 0;

template <functionHookLocation at>
static void fn_vMakeCharacterReact() {
  if (at == S) {
    // Start of function.
    emulator::pushProcessorState();
    
    if (GPR(3) == 0x80BF0C0C) {
      //updateDynamics(pointer<stSuperObject, true>(GPR(3)));
      bStep2(pointer<stSuperObject, true>(GPR(3)));
    }
    
//    uint32_t dt = 1000.0f / (60.0f / float(engineLoopCount));
//    // To account for fluctuations of the framerate
//    if (engineLoopCurrent % engineLoopCount == 0) dt -= engineLoopCount;
//    // This works well for the normal walk cycle, but there are still
//    // problems while strafing (much faster) and pulling up from a
//    // ledge (rayman is launched into the air). Good enough though.
//    game::g_stEngineStructure->timer.deltaCount = game_byteorder_32(dt);
//    game::g_stEngineStructure->timer.usefulDeltaTime = game_byteorder_32(dt);
    
  } else if (at == E) {
    // Normal return behavior
    uint32_t sp = GPR(1) & 0x7FFFFFFF; // sp = r1
    GPR(29) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x14));
    GPR(30) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x18));
    GPR(31) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x1C));
    GPR(0)  = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x24)); // lr = r0
    GPR(1) += 32; // align sp
    NPC = LR = GPR(0); // jump
    
    //stSuperObject *superObject = pointer<stSuperObject, true>(GPR(3) - 0x80);
    // Physics have been processed for the object.
    if (GPR(3) - 0x80 == 0x80BF0C0C) {
      //saveCalculatedState(pointer<stSuperObject, true>(GPR(3) - 0x80));
      bStepDone2(pointer<stSuperObject, true>(GPR(3) - 0x80));
      if (++engineLoopCurrent < engineLoopCount) {
        // Jump back to start
        emulator::popProcessorState();
        NPC = 0x8003b248;
      } else {
        engineLoopCurrent = 0;
        
        printf("num blocks: %d\n", blocks.size());
        printf("average iterations: %f\n", averageNumIterations);
        printf("time per iteration: %f\n", (float)(clock() - clocktt) / (float)CLOCKS_PER_SEC);
        
        clocktt = clock();
      }
    }
  }
}

template <functionHookLocation at>
static void fn_vDynamicsBaseMechanics() {
  if (at == S) {
    // Start of function.
    emulator::pushProcessorState();
    
    if (GPR(3) == 0x80BF0C0C) {
      //updateDynamics(pointer<stSuperObject, true>(GPR(3)));
    }
    
//    uint32_t dt = 1000.0f / (60.0f / float(engineLoopCount));
//    // To account for fluctuations of the framerate
//    if (engineLoopCurrent % engineLoopCount == 0) dt -= engineLoopCount;
//    // This works well for the normal walk cycle, but there are still
//    // problems while strafing (much faster) and pulling up from a
//    // ledge (rayman is launched into the air). Good enough though.
//    game::g_stEngineStructure->timer.deltaCount = game_byteorder_32(dt);
//    game::g_stEngineStructure->timer.usefulDeltaTime = game_byteorder_32(dt);
    
  } else if (at == E) {
    // Normal return behavior
    uint32_t sp = GPR(1) & 0x7FFFFFFF; // sp = r1
    GPR(29) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x14));
    GPR(30) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x18));
    GPR(31) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x1C));
    GPR(0)  = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x24)); // lr = r0
    GPR(1) += 32; // align sp
    NPC = LR = GPR(0); // jump
    
    //stSuperObject *superObject = pointer<stSuperObject, true>(GPR(3) - 0x80);
    // Physics have been processed for the object.
    if (GPR(3) - 0x80 == 0x80BF0C0C) {
      //saveCalculatedState(pointer<stSuperObject, true>(GPR(3) - 0x80));
      if (++engineLoopCurrent < engineLoopCount) {
        // Jump back to start
        emulator::popProcessorState();
        NPC = 0x8003b248;
      } else {
        engineLoopCurrent = 0;
        
        printf("time per iteration: %f\n", (float)(clock() - clocktt) / (float)CLOCKS_PER_SEC);
        
        clocktt = clock();
      }
    }
  }
}


static void timingStart()
{
    emulator::pushProcessorState();
}

static void timingEnd()
{
    uint32_t sp = GPR(1) & 0x7FFFFFFF; /* sp = r1 */
    //GPR(29) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x14));
    //GPR(30) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x18));
    GPR(31) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x1C));
    GPR(0)  = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x24)); /* lr = r0 */
    GPR(1) += 32; /* SP alignment */
    NPC = LR = GPR(0); // jump
    
    if (++engineLoopCurrent < engineLoopCount)
    {
        emulator::popProcessorState();
        //NPC = 0x80034ca0;
    }
    else
    {
        /* Loops ended */
        engineLoopCurrent = 0;
    }
}

bruteforcingTool::bruteforcingTool()
{
//  attempt a;
//  attempts.push_back(a);
//  attempts.push_back(a);
//  attempts.push_back(a);
//
  attempts.reserve(10);
  
   // hook<0x8009eddc>(hijack);
    
    //hook<0x80034ca0>(timingStart);
   // hook<0x80034ec4>(timingEnd, hookType::replace);
    
    //hook<0x80034ca0>(timingFunctionStart);
    //hook<0x80034ec4>(timingFunctionEnd, hookType::replace);
    
    //hook<0x80036c3c>(changeTreatedFrame); // Dynamic hierarchy
    //hook<0x80036fd4>(changeTreatedFrame); // Camera hierarchy
    
    //hook<0x80037178>(matrixStack);
    //hook<0x80034cf8>(hijackTimingFunction);
    
    //hook<0x80035c9c>(&test);
    //hook<0x80035bd4>(&frameStart);
    //hook<0x80035edc>(&frameEnd, hookType::replace);
    //hook<0x80036174>(&displayAll, hookType::replace);
    
    //hook<0x8009f17c>(&timingHook);
  
//  hook<0x8003b054>(fn_vMakeCharacterThink<S>, hookType::replace);
//  hook<0x8003b13c>(fn_vMakeCharacterThink<E>, hookType::start);
//
  
  
  
  //hook<0x8003b248>(testing2, hookType::start);
  
  hook<0x8003b248>(fn_vMakeCharacterReact<S>, hookType::start);
  hook<0x8003b350>(fn_vMakeCharacterReact<E>, hookType::replace);
  
  
//  hook<0x8003b248>(fn_vDynamicsBaseMechanics<S>, hookType::start);
//  hook<0x8003b350>(fn_vDynamicsBaseMechanics<E>, hookType::replace);
  
  
  
  
  
  
//
//    hook<0x8003b248>(&makeCharacterReactStart);
//    hook<0x8003b350>(&makeCharacterReact, hookType::replace);
//
    //hook<0x80037078>(&engineStart, hookType::start);
    //hook<0x8003748c>(&engineEnd, hookType::replace);
    
    //e(0x80037078, "hook_pre_engine", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_pre_engine);
        //hook_create(0x8003748c, "
}


//#include <stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#include <time.h>
//#include <dispatch/dispatch.h>
//
//static bool bruteforce_on = false;
//static unsigned int frameCounter = 0;
//static unsigned int frameCounterLast = 0;
//static unsigned int numStatesCurrent = 0;
//static unsigned int numStatesAllocated = 0;
//static struct State state[1000000];
//
//struct State CurrentGameState()
//{
//    struct State S;
//    /* Copy global superobject matrix */
//    //S.globalMatrix = fnActorGet
//    /* Copy dynamics state */
//    S.dynamics = *fnActorGetDynamics(actor_rayman);
//    //S.dynamics.base.report = 0;
//    S.frame = host_byteorder_32(engine->timer.frame);
//    S.score = vector3_length(vector3_sub(fnActorGetPosition(actor_rayman), fnActorGetPosition(actor_changemap)));
//
//    return S;
//}
//
//struct State LastState()
//{
//    return state[numStatesCurrent-1];
//}
//
//void PushPlayerState(struct State S)
//{
//    state[numStatesCurrent++] = S;
//}
//
//void RestorePlayerState(struct State S)
//{
//    memcpy(fnActorGetDynamics(actor_rayman), &S.dynamics, sizeof S.dynamics);
//}
//
//static float lowest = INFINITY;
//
////struct State NewState()
////{
////
////}
//
//static unsigned int framesToWait = 0;
//
//static int frame = 0;
//
//
//static long last = 0;
//
//void BruteforceInput()
//{
//    //printf("hi2\n");
//    //engine->timer.timerHandle = 0;
//    //engine->inputMode = 1;
//    //input.stick.main.x->analogValue = game_byteorder_f32(100);
//    //printf("reset\n");
//    //engine->timer.ticksPerMs = game_byteorder_32(40500);
//    //engine->timer.frame = game_byteorder_32(host_byteorder_32(engine->timer.frame) + 1);
////    stInputStructure *st = (stInputStructure*)(memoryBase + 0x42F5BC);
////    stInputDevice *controller = &st->device[0];
////    controller->joypadCounter[0] = 1;
//
//    //printf("before: %d\n", host_byteorder_32(engine->timer.totalRealTimeLow));
//
////    if (frame++ < 1)
////    {
////        input.stick.main.x->analogValue = game_byteorder_f32(100.0f);
////        //printf("Press\n");
////    }
////    else
////    {
////        //input.stick.main.x->analogValue = game_byteorder_f32(0.0f);
////    }
//
////    for (int i = 0; i < 16; i++)
////    {
////        engine->timer.counter[i] = game_byteorder_32(host_byteorder_32(engine->timer.counter[i]) + 1);
////    }
////
//    //printf("hi!\n");
//
//    //printf("a %d\n", frame);
////    if (frame++ == 2 && input.stick.main.x)
////    {
////        input.stick.main.x->analogValue = game_byteorder_f32(100.0f);
////        printf("Press\n");
////    }
////    else if (input.stick.main.x && frame > 6)
////    {
////        //input.stick.main.x->analogValue = game_byteorder_f32(0.0f);
////    }
//
//
////    else if (input.stick.main.x && frame == 14)
////    {
////        input.stick.main.x->analogValue = game_byteorder_f32(0.0f);
////    }
//
//    //printf("hi!\n");
//
//
////    if (input.stick.main.x){
////        input.stick.main.x->analogValue = game_byteorder_f32(drand48() * 200.0f - 100.0f);
////        input.stick.main.y->analogValue = game_byteorder_f32(drand48() * 200.0f - 100.0f);
////    }
//
//    //engine->timer.frame = game_byteorder_32(host_byteorder_32(engine->timer.frame) + 1);
//
//    //engine->timer.totalRealTimeLow = game_byteorder_32(0);
//    //printf("last: %d\n", last);
//
//    printf("engine start hook\n");
//}
//
//int i = 0;
//
//static bool requestLoadstate = false;
//
//static uint8 *savestate = NULL;
//static void FastLoadState()
//{
//    //memcpy(memoryBase, savestate, sizeof(uint8) * 24 * 1000 * 500);
//}
//
//void BruteforceMain(int *numLoops)
//{
//    *numLoops = 0;
//
//    printf("engine end hook\n");
////
////        if (bruteforce_on)
////        {
////            //memset(emulatorPadStatus, 0, sizeof (struct GCPadStatus));
////
////
////            //RestorePlayerState(LastState());
////
////            //dispatch_sync(dispatch_get_global_queue(<#intptr_t identifier#>, <#uintptr_t flags#>), <#^(void)block#>)
////            //emulatorPadStatus->button = rand() % UINT16_MAX;
////
//////            if (frameCounter < frameCounterLast)
//////            {
//////                printf("playing back frame %d / %d\n", frameCounter, frameCounterLast);
//////                *emulatorPadStatus = state[frameCounter].gamepad;
//////
//////                //emulatorPadStatus->stickX = arc4random() % 255;
//////                //emulatorPadStatus->stickY = arc4random() % 255;
//////                //if ()
//////
//////                frameCounter++;
//////            }
//////            else
////            {
////                //frameCounter = 0;
////
////                /* Set the gamepad to a random state */
////                emulatorPadStatus->stickX = arc4random() % 255;
////                emulatorPadStatus->stickY = arc4random() % 255;
////                /* Wait one frame until inputs go through */
////                framesToWait = 3;
////
////                printf("Press: %d\n", host_byteorder_32(engine->timer.frame));
////
////
////
//////                emulatorPadStatus->substickX = rand() % 255;
//////                emulatorPadStatus->substickY = rand() % 255;
////
////                //if ((frameCounter % 10) == 0) emulatorLoadState(1);
////
////
////                //PushPlayerState(S);
////
//////                stEngineObject *rayman = actor_rayman;
//////                stEngineObject *endMap = actor_changemap;
//////
//////                float d = vector3_length(vector3_sub(fnActorGetPosition(rayman), fnActorGetPosition(endMap)));
//////                //float t = (float)host_byteorder_32(engine->timer.frame) / d;
//////                if (d < state[frameCounter].score)
//////                {
//////
//////                    lowest = d;
//////
//////                    struct State S = CurrentGameState();
//////                    S.gamepad = *emulatorPadStatus;
//////                    PushPlayerState(S);
//////
//////                    frameCounter=0;
//////                    //frameCounterLast++;
//////                    frameCounterLast++;
//////                    emulatorLoadState(1);
//////                    //emulatorLoadState(1);
//////                    //emulatorSaveState(1, true);
//////                }
//////                else
//////                {
//////                    //emulatorLoadState(1);
//////                }
////
////
////                //input.stick.main.x->analogValue = game_byteorder_f32(-100.0f);
////
////
////                //stVector3D p = fnActorGetPosition(rayman);
////                //printf("pos: %.2f %.2f %.2f\n", p.x, p.y, p.z);
////            }
////
////            //frameCounter++;
////        }
////        else
////        {
////            lowest = vector3_length(vector3_sub(fnActorGetPosition(actor_rayman), fnActorGetPosition(actor_changemap)));
////        }
//
//}
//
//
//static enum StateMachineState
//{
//    S_WAIT,
//    S_INPUT,
//    S_INPUT_REGISTER,
//    S_FIND_FIRST_FRAME,
//    S_SEEK_FIRST_FRAME,
//} CurrentState = S_FIND_FIRST_FRAME, NextState;
//
//static int WaitDuration = 0;
//static unsigned LastFrameHandled = 0;
//static float BestDistance = 0.0f;
//
//static void RandomInput(struct GCPadStatus *gamepad)
//{
//    bool a = rand() % 10;
//    bool b = rand() % 10;
//
//    if (a) gamepad->button |= GAMEPAD_BUTTON_A;
//    if (b) gamepad->button |= GAMEPAD_BUTTON_B;
//    gamepad->stickX = rand() % 255;
//    gamepad->stickY = rand() % 255;
//}
//
//void BruteforceStateHandler(struct GCPadStatus *gamepad)
//{
//
//begin:
//
//    //printf("hi!\n");
//    if (hierarchy)
//    {
//        engine->timer.ticksPerMs = game_byteorder_32(40500/100);
//
//        /* Only the last polled input is ever handled */
//        if (host_byteorder_32(engine->timer.frame) == LastFrameHandled)
//        {
//            switch (CurrentState)
//            {
//                case S_WAIT:
//                {
//                    WaitDuration--;
//                    if (WaitDuration-1 <= 0)
//                    {
//                        CurrentState = NextState;
//                    }
//                    printf("wait: %d\n", WaitDuration);
//                    break;
//                }
//
//                case S_INPUT_REGISTER:
//                {
//                    float d = vector3_length(vector3_sub(fnActorGetPosition(actor_rayman), fnActorGetPosition(actor_changemap)));
//
//                    if (d < BestDistance)
//                    {
//                        emulatorLoadState(1);
//                        printf("best reached: %f\n", d);
//                        BestDistance = d;
//
//                        CurrentState = S_INPUT;
//                        goto begin;
//                    }
//
//                    break;
//                }
//
//                case S_INPUT:
//                {
//                    RandomInput(gamepad);
//
//                    WaitDuration = 1;
//                    CurrentState = S_WAIT;
//                    NextState = S_INPUT_REGISTER;
//                    break;
//                }
//
//                case S_FIND_FIRST_FRAME: // Find first available input frame
//                {
//                    //printf("%s\n", fnActorGe(actor_rayman));
//                    char* S = fnActorGetCurrentBehaviorName(actor_rayman);
//                    if (S) S = strchr(S, ':') + 1;
//
//                    /* All buttons available here */
//    //                if (strcmp("YAM_C_Attente", S) == 0)
//    //                {
//    //                    printf("save!\n");
//    //                    //gamepad->stickX = 255;
//    //                    /* Save state and return S_WAITINg */
//    //                    emulatorSaveState(1, false);
//    //                    CurrentState = S_INPUT;
//    //                }
//
//                    /* Strafe lock available on YAM_C_Init */
//                    if (strcmp("YAM_C_Init", S) == 0)
//                    {
//                        // TODO: Determine if strafe locking should be done for faster initial turn angle
//                        //gamepad->button = GAMEPAD_TRIGGER_R;
//
//                        BestDistance = vector3_length(vector3_sub(fnActorGetPosition(actor_rayman), fnActorGetPosition(actor_changemap)));
//
//                        CurrentState = S_INPUT;
//
//                        emulatorSaveState(1, true);
//                    }
//
//                    break;
//                }
//
//                case S_SEEK_FIRST_FRAME:
//                {
//
//                }
//
//                default:
//                    break;
//            }
//        }
//
//        LastFrameHandled = host_byteorder_32(engine->timer.frame);
//
//    }
//
//    //printf("hi!\n");
//
////    srand(time(0));
//    //gamepad->stickX = drand48() * 255;
//    //gamepad->stickY = drand48() * 255;
////
////    if (frameCounter++ >= 3)
////    {
////        if (frameCounter == 4)
////        {
////            float d = vector3_length(vector3_sub(fnActorGetPosition(actor_rayman), fnActorGetPosition(actor_changemap)));
////            printf("d: %f\n", d);
////        }
////        else if (frameCounter == 6)
////        {
////            //FastLoadState();
////            emulatorLoadState(1);
////            frameCounter = 0;
////
////
////        }
////
////       // FastLoadState();
////    }
//
////    if (requestLoadstate)
////    {
////        emulatorLoadState(1);
////        requestLoadstate = false;
////    }
//
////    if (input.stick.main.x) {
////        input.stick.main.x->analogValue = game_byteorder_f32(drand48() * 200.0f - 100.0f);
////        input.stick.main.y->analogValue = game_byteorder_f32(drand48() * 200.0f - 100.0f);
////    }
//}
//
//
//void BruteforceBegin()
//{
//    // 803e7ccc
//
//    emulatorSaveState(1, true);
//
//    //last = host_byteorder_32(engine->timer.totalRealTimeLow);
//
//    //engine->timer.ticksPerMs = game_byteorder_32(40500 / 10);
//
//   // srand(time(0));
//    frameCounter = 0;
//    //savestate = malloc(sizeof(uint8) * 24 * 1000 * 500);
//    //memcpy(savestate, memoryBase, sizeof(uint8) * 24 * 1000 * 500);
//    //PushPlayerState(CurrentGameState());
//    bruteforce_on = true;
//}
//
//void BeforeEngineExecuted(int *numLoops)
//{
//    //engine->inputMode=1;
//    //*numLoops = 0;
//
//    //engine->timer.ticksPerMs = game_byteorder_32(40500/2);
//
////    if (last == 0)
////    {
////        last = host_byteorder_32(engine->timer.totalRealTimeLow);
////    }
////    else
////    {
////        long diff = host_byteorder_32(engine->timer.totalRealTimeLow) - last;
////        //printf("diff: %ld\n", diff);
////        engine->timer.totalRealTimeLow = game_byteorder_32(last + diff*2);
////    }
////
////    last = host_byteorder_32(engine->timer.totalRealTimeLow);
////
////
////    //printf("Before engine execution\n");
//    if (frame < 1)
//    {
//        input.stick.main.x->analogValue = game_byteorder_f32(/*drand48() * 200.0f - 100.0f*/100.0f);
//        //input.stick.main.y->analogValue = game_byteorder_f32(drand48() * 200.0f - 100.0f);
//        //printf("press\n");
//    }
//
//    frame++;
////
////    printf("input: %d\n", input.stick.main.x->analogValue);
//}
//
//void AfterEngineExecuted()
//{
//
//    //printf("After engine execution\n");
//
//    input.stick.main.x->analogValue = 0;
//}
//
//static int aaaf = 0;
//
//static bool cycle()
//{
//    aaaf++;
//    if ((aaaf % 50) == 0) return false;
//    return true;
//}
//
//
//const struct BruteforcingTool * bruteforcer = &(const struct BruteforcingTool)
//{
//    &cycle,
//};
