//
//  bruteforce.c
//  r3-tas
//
//  Created by Jba03 on 2023-06-26.
//

#include "bruteforce2.hh"
#include "game.hh"
#include "structure.hh"
#include "hook.hh"

#define radians(deg) ((deg) * (M_PI / 180.0f))
#define degrees(rad) ((rad) * (180.0f / M_PI))

#include <random>

static int currentIteration = 0;
static int maxIterations = 100;

bState block, cameraBlock;
bool saveRequested = false;
bool loadRequested = false;

#pragma mark - Tools

static inline float rround(float v, float precision = 1.0f) {
  return std::round(v / precision) * precision;
}

static const uint64_t getRoughPositionHash(stVector3D pos) {
  float x = rround(float(pos.x), 0.1f);
  float y = rround(float(pos.y), 0.1f);
  float z = rround(float(pos.z), 0.1f);
  
  uint64_t qx = std::hash<float>()(x);
  uint64_t qy = std::hash<float>()(y);
  uint64_t qz = std::hash<float>()(z);
  
  //return (qx ^ (qy << 1)) ^ qz;
  return (qx << 32) | qy | (qz << 24);
}

static auto randomJoyStick() -> float {
  return (drand48() * 2.0f - 1.0f) * 100.0f;
}

#pragma mark - Bruteforce

static auto input(bInputState *input) -> void {
  // Disable controller input
  game::g_stEngineStructure->inputMode = stEngineStructure::inputMode::commands;  //stEngineStructure::inputMode::commands;
  
  game::g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeX].analogValue = input->mJoyX;
  game::g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeY].analogValue = input->mJoyY;
  game::g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeV].analogValue = input->cJoyX;
  game::g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeZ].analogValue = input->cJoyY;
  
//  game::IPT_g_stInputStructure->entries[IPT_E_Entry_Action_Sauter].state = game_byteorder_32(rand()%30);
//  game::IPT_g_stInputStructure->entries[IPT_E_Entry_Action_Helico].state = game_byteorder_32(rand()%5);
  
  //game::IPT_g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeV].analogValue = game_byteorder_f32(128.0f);
  
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

static bool finished = false;
static bool firstIteration = true;
//std::map<uint64_t, bState> blocks;
std::map<uint64_t, bState> blocks;
static bState *selectedBlock = nullptr;
bState *initialBlock = nullptr;
static float averageNumIterations = 0.0f;
static float selectionBias = 1.0f;
static stVector3D startPosition;
static float baseDistance = INFINITY;

static const stVector3D target = stVector3D(-34, 11.5, -5.10);
//stVector3D(-17.91, 21.59, -4.96);
//stVector3D(-86.23, 19.94, -7.92);
//stVector3D(63.79, 21.01, -2.82);
static const uint64_t targetHash = getRoughPositionHash(target);

static auto saveCurrentState(stDynamics *dynamics, stMind *mind) -> void {
  
}

static auto bStepBegin(stSuperObject *spo) -> void {
  if (firstIteration) {
    bState state;
    stVector3D position = spo->globalTransform->position();
    uint64_t hash = getRoughPositionHash(position);
    state.depth = 0;
    
    blocks[hash] = state;
    
    // Initialize some global variables
    initialBlock = &blocks[hash];
    startPosition = position;
  }
  
  // Select a random block from the map.
  selectedBlock = nullptr;
  while (selectedBlock == nullptr) {
    auto it = blocks.begin();
    std::advance(it, rand() % blocks.size());
    selectedBlock = &blocks[it->first];
    //printf("random block: %p\n", selectedBlock);
  }
}

static auto bStepInput(stSuperObject *spo) -> void {
  bInputState state;
  state.mJoyX = randomJoyStick();
  state.mJoyY = randomJoyStick();
  
  selectedBlock->setInputState(state);
  //input(&state);
}

static auto bStepIntelligence(stSuperObject *spo) -> void {
  
}

bState currentState;
uint64_t initialHash;
float averageDistancePerDepth[10000];

int iter = 0;

static auto bStepDynamics(stSuperObject *spo, stDynamics *dyn) -> void {
  
  //printf("set first iteration\n");
  
  if (finished) return;

  if (firstIteration) {
    // Insert the block for the initial position
    bState state;
    state.setDynamics(dyn);
    state.setTransform(spo->globalTransform);
    state.depth = 0;
    
    uint64_t hash = getRoughPositionHash(dyn->base.currentTransform.position());
    initialHash = hash;
    blocks[hash] = state;
    
    initialBlock = &blocks[hash];
//
//    for (int i = 0; i < 10000; i++)
//      averageDistancePerDepth[i] = (dyn->base.currentTransform.position() - target).length();
//
    firstIteration = false;
  }
  
//  iter++;
//  if ((iter % 100) == 0) {
//    auto it = blocks.begin();
//    while (++it != blocks.end()) {
//
//      bState *state = &blocks[it->first];
//
//      bState *chain = initialBlock;
//      bool isPartOfChain = false;
//      while (chain->next != nullptr) {
//        if (state == chain) isPartOfChain = true;
//        chain = chain->next;
//      }
//
//
//      if (!isPartOfChain)
//        blocks.erase(it);
//
//      //it++;
//
//
//    }
//  }
  
    
    selectedBlock = nullptr;
    while (selectedBlock == nullptr) {
      auto it = blocks.begin();
      std::advance(it, rand() % blocks.size());
      selectedBlock = &blocks[it->first];
      
      // Don't travel backwards (for now)
      if ((selectedBlock->physicsState.dynamics.base.currentTransform.position() - target).length() > baseDistance)
        selectedBlock = nullptr;
      
      if (selectedBlock->depth < averageNumIterations && rand() % 10 == 0) {
        selectedBlock = nullptr;
        blocks.erase(it);
      }
      
//        if (selectedBlock->timesLoaded >= 10)
//          selectedBlock = nullptr;
        
      if (selectedBlock) {
        if (selectedBlock->depth < averageNumIterations - 20) {
          selectedBlock = nullptr;
        }
      }
        
        //      if (selectedBlock->next == nullptr) {
        //        while (selectedBlock != initialBlock) {
        //
        //          auto it = blocks.find(getRoughPositionHash(selectedBlock->physicsState.dynamics.base.currentTransform.position()));
        //          //if (it != blocks.end()) blocks.erase(it);
        //
        //          selectedBlock = selectedBlock->prev;
        //        }
        //      }
        
        //
        
//        if ((selectedBlock->physicsState.dynamics.base.currentTransform.position() - target).length() > averageDistancePerDepth[selectedBlock->depth]) {
//          //printf("erase\n");
//          selectedBlock = nullptr;
//          //blocks.erase(it);
//        }
//
//              if (selectedBlock->depth < averageNumIterations - 5.0f /*&& selectedBlock->next == nullptr*/) {
//                //printf("erase: %p\n", it);
//                //blocks.erase(it);
//                //printf("erase2\n");
//                selectedBlock = nullptr;
//              }
    }
  
  //printf("average depth: %f (%lu blocks)\n", averageNumIterations, blocks.size());
  
  //printf("selected block %p with depth %d (out of %lu)\n", selectedBlock, selectedBlock->depth, blocks.size());

  // Load the selected block
  selectedBlock->load(spo, dyn);
  
  stVector3D p = dyn->base.currentTransform.position();
  //printf("before move: (%.2f, %.2f, %.2f, %llX)\n", float(p.x), float(p.y), float(p.z), getRoughPositionHash(p));
  
  
  float x = 0.0f, y = 0.0f;
//  float angle = 180.0f;
//  // Small bias towards the angle
//  while (fabs(angle) > 5.0f)
//  {
//    x = float(16.0f * (drand48() * 2.0f - 1.0f));
//    y = float(16.0f * (drand48() * 2.0f - 1.0f));
//
//    const float sA = atan2(y, x);
//
//    stVector3D pA = dyn->base.currentTransform.position();
//    stVector3D pB = target;
//
//    const float hR = atan2(float(pB.y) - float(pA.y), float(pB.x) - float(pA.x));
//    // Then, calculate the new, relative angle of both
//    angle = degrees(hR - sA);
//  }
  
  stVector3D pA = dyn->base.currentTransform.position();
  stVector3D pB = target;
  float hR = atan2f(float(pB.y) - float(pA.y), float(pB.x) - float(pA.x));
  
  float limit = 10.0f;
  float rnd = (drand48() * 2.0f - 1.0f) * limit;
  hR += radians(rnd);
  
  
  float strength = 16.0f;// - drand48() * 8.0f;
  x = cosf(hR) * strength;
  y = sinf(hR) * strength;
  
  
  dyn->base.animationProposeSpeed.x = x;
  dyn->base.animationProposeSpeed.y = y;
  
  
  //printf("move direction: %f\n", float(dyn->base.animationProposeSpeed.y) / float(dyn->base.animationProposeSpeed.x));
}

static auto bStepEnd(stSuperObject *spo) -> void {
  
  stEngineObject *obj = spo->data;
  stDynam *dynam = obj->dynam;
  stDynamics *dynamic = dynam->dynamics;
  
  stVector3D p = dynamic->base.currentTransform.position();
  
  //printf("after move: (%.2f, %.2f, %.2f, %llX)\n\n", float(p.x), float(p.y), float(p.z), getRoughPositionHash(p));
  
  // Get the current position hash
  uint64_t hash = getRoughPositionHash(p);
  
  // Check if there is already a block at this position
  if (blocks.find(hash) != blocks.end()) {
    bState *block = &blocks.at(hash);
    // There is a block for this position.
    // Was it reached faster than the previous time?
    if (selectedBlock->depth + 1 < block->depth) {
      // If so, tell the block it was reached faster.
      block->depth = selectedBlock->depth + 1;
      
      // Branch cut-off
//      if (selectedBlock->next) {
//        bState *branchDepth = selectedBlock->next;
//       // while (branchDepth) {
//          uint64_t hash = getRoughPositionHash(branchDepth->physicsState.dynamics.base.currentTransform.position());
//          //branchDepth = branchDepth->next;
//          auto it = blocks.find(hash);
//          if (it != blocks.end()) blocks.erase(it);
//        //}
//        //auto it = blocks.find(hash);
//
//      }
      
      //printf("reached faster\n");
      // And set the next node of the current block
      selectedBlock->next = block;
      //block->prev = selectedBlock;
      
      averageDistancePerDepth[block->depth] += (selectedBlock->physicsState.dynamics.base.currentTransform.position() - target).length();
      averageDistancePerDepth[block->depth] /= 2.0f;
      
    }
  } else {
    //printf("create a block for %llx\n", hash);
    // There is no block for this position, so we need to create it.
    bState state = {};
    state.setDynamics(dynamic);
    state.setTransform(spo->globalTransform);
    state.depth = selectedBlock->depth + 1;

    //printf("reached faster: %d\n", state.depth);
    //state.next = selectedBlock;
    blocks[hash] = state;
    bState *s = &blocks[hash];
//    if (s == nullptr) printf("aaa\n");
    selectedBlock->next = s;
    
    //printf("iteration reached: %d\n", state.iterationReached);
    averageNumIterations += state.depth;
    averageNumIterations /= 2.0f;

    //averageNumIterations = 2;
  }
  
  if (getRoughPositionHash(dynamic->base.currentTransform.position()) == targetHash) {
    finished = true;
    maxIterations = 1;
  }
  
}


#pragma mark - Hook

enum functionHookLocation { S, E };

static auto fn_vMakeCharacterThink() -> void {
  if (GPR(3) == 0x80BF0C0C) {
    stSuperObject *superObject = pointer<stSuperObject>(memory::address(GPR(3)));
    bStepBegin(superObject);
    bStepInput(superObject);
    bStepIntelligence(superObject);
    
//    if (loadRequested) {
//      stSuperObject *spo = pointer<stSuperObject, true>GPR(3);
//      block.intelligenceState.load(spo);
//    }
  }
}

static auto fn_s_vMakeCharacterReact() -> void {
  emulator::pushProcessorState();
}

clock_t clk = 0;

static auto fn_e_vMakeCharacterReact() -> void {
  uint32_t spo = GPR(31);
  // Physics have been processed for the object.
  if (spo == 0x80BF0C0C /*|| spo == 0x80C533E0*/) {
    stSuperObject *superObject = pointer<stSuperObject>(memory::address(spo));
    bStepEnd(superObject);
    if (++currentIteration < maxIterations) {
      // Jump back to start
      emulator::popProcessorState();
      NPC = 0x8003b248;
      return;
    } else {
      currentIteration = 0;
      
      
      printf("avg: %f, time: %f\n", averageNumIterations, (float)(clock() - clk) / (float)CLOCKS_PER_SEC);
      
      clk = clock();
    }
  }
  
  // Normal return behavior
  uint32_t sp = GPR(1) & 0x7FFFFFFF; // sp = r1
  // if anything goes wrong here, the swap function is most likely the issue
  GPR(29) = memory::swap(*(uint32_t*)(memory::baseAddress + sp + 0x14));
  GPR(30) = memory::swap(*(uint32_t*)(memory::baseAddress + sp + 0x18));
  GPR(31) = memory::swap(*(uint32_t*)(memory::baseAddress + sp + 0x1C));
  GPR(0)  = memory::swap(*(uint32_t*)(memory::baseAddress + sp + 0x24)); // lr = r0
  GPR(1) += 32; // align sp
  NPC = LR = GPR(0); // jump
}

template <functionHookLocation at>
static void fn_vDynamicsBaseMechanics() {
  if (at == S) {
    
    
    
    ///printf("spo?: %X\n", memory::address(GPR(4)).effectiveAddress());
    
    stSuperObject *spo = pointer<stSuperObject>(memory::address(GPR(4)));
    stDynamics *dynamic = pointer<stSuperObject>(memory::address(GPR(3)));
    stEngineObject *obj = spo->data;
    
    if (GPR(4) == 0x80BF0C0C) {
      
      //printf("%f\n", game::g_stEngineStructure->timer.frameLength.f());
      //game::g_stEngineStructure->timer.frameLength = game_byteorder_f32(0.016667 * float(maxIterations));
      
      //stVector3D impose2 = dynamic->base.imposeSpeed = st;
      
      //dynamic->base.previousSpeed = stVector3D();
      
      //float step = 1.69164f;
      //float step = 1.5164f;
      //saveCurrentState
      
      bStepDynamics(spo, dynamic);
      
      float x = (drand48() - 0.5f) * 2.0f;
      float y = (drand48() - 0.5f) * 2.0f;
      float z = (drand48() - 0.5f) * 2.0f;
      //dynamic->base.animationProposeSpeed = stVector3D(x, y, 0.0f) * 16.0f * 10.0f;
      
      
      // The following works when walking, not rolling
      float step = 1.037f + /*(1.0f / 3.0f)*/ 0.41f * (maxIterations+1);
      //stVector3D newSpeed = dynamic->base.animationProposeSpeed * step;
      
      
//      dynamic->base.proposeSpeed = stVector3D();
//      dynamic->base.imposeSpeed = stVector3D();
//      dynamic->base.previousSpeed = stVector3D();
//      dynamic->advanced.addedSpeed = stVector3D();
//      dynamic->base.addedTranslation = stVector3D();
      
      // for corrected? speed
      //dynamic->base.animationProposeSpeed = newSpeed; //stVector3D();
      
      
//      dynamic->advanced.inertiaTranslation = stVector3D();
      
      stVector3D pos = dynamic->base.currentTransform.position();
      stVector3D propose = dynamic->base.proposeSpeed;
      stVector3D impose = dynamic->base.imposeSpeed;
      stVector3D previous = dynamic->base.previousSpeed;
      stVector3D added = dynamic->advanced.addedSpeed;
      stVector3D addedTranslation = dynamic->base.addedTranslation;
      stVector3D animationSpeed = dynamic->base.animationProposeSpeed;
      stVector3D inertiaTranslation = dynamic->advanced.inertiaTranslation;
      
//      if (saveRequested) {
//        bIntelligenceState intel(spo);
//        
//        block.setIntelligenceState(intel);
//        block.setDynamics(dynamic);
//        block.setTransform(spo->globalTransform);
//      }
//
//      if (loadRequested) {
//        block.load(spo, dynamic);
//      }
      
//
      //dynamic->base.flags = game_byteorder_32(0x700A25);
      
//      printf("position: %f %f %f\n", pos.x, pos.y, pos.z);
//      printf("flags: %X, %X\n", (uint32_t)dynamic->base.flags, (uint32_t)dynamic->base.endFlags);
//      printf("propose: %.2f %.2f %.2f\n", propose.x, propose.y, propose.z);
//      printf("impose: %.2f %.2f %.2f\n", impose.x, impose.y, impose.z);
//      printf("previous: %.2f %.2f %.2f\n", previous.x, previous.y, previous.z);
//      printf("added: %.2f %.2f %.2f\n", added.x, added.y, added.z);
//      printf("addedTranslation: %.2f %.2f %.2f\n", addedTranslation.x, addedTranslation.y, addedTranslation.z);
//      printf("animationSpeed: %f %f %f\n", animationSpeed.x, animationSpeed.y, animationSpeed.z);
//      printf("inertiaTranslation: %.2f %.2f %.2f\n", inertiaTranslation.x, inertiaTranslation.y, inertiaTranslation.z);
//      printf("\n");
      
    }
  }
}

template <functionHookLocation at>
static auto fn_vDynamicsCameraMechanics() -> void {
  if (at == S) {
    stDynamics *dynamic = pointer<stSuperObject>(memory::address(GPR(3)));
    stSuperObject *spo = pointer<stSuperObject>(memory::address(GPR(4)));
    
    /*if (GPR(4) == 0x80C533E0)*/ {
//      if (saveRequested) {
//        bIntelligenceState intel(spo);
//        cameraBlock.setIntelligenceState(intel);
//        cameraBlock.setDynamics(dynamic);
//        cameraBlock.setTransform(spo->globalTransform);
//
//        saveRequested = false;
//      }
//
//      if (loadRequested) {
//        cameraBlock.load(spo, dynamic);
//
//        loadRequested = false;
//      }
    }
  }
}

bruteforcingTool::bruteforcingTool()
{
  hook<0x8003b054>(fn_vMakeCharacterThink, hookType::start);
//
  hook<0x8003b248>(fn_s_vMakeCharacterReact, hookType::start);
  hook<0x8003b350>(fn_e_vMakeCharacterReact, hookType::replace);
//
//
  hook<0x8007e9d8>(fn_vDynamicsBaseMechanics<S>, hookType::start);
//  hook<0x8007d714>(fn_vDynamicsCameraMechanics<S>, hookType::start);
  //hook<0x8003b350>(fn_vDynamicsBaseMechanics<E>, hookType::replace);
  
  
  
//  hook<0x80034ca0>(timingFunctionStart);
//  hook<0x80034ec4>(timingFunctionEnd, hookType::replace);
  
  
//
//    hook<0x8003b248>(&makeCharacterReactStart);
//    hook<0x8003b350>(&makeCharacterReact, hookType::replace);
//
//    hook<0x80037078>(&engineStart, hookType::start);
//    hook<0x8003748c>(&engineEnd, hookType::replace);
    
    //e(0x80037078, "hook_pre_engine", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_pre_engine);
        //hook_create(0x8003748c, "
}
