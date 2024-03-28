//
//  emulator.c
//  r3-tas
//
//  Created by Jba03 on 2023-05-06.
//

#include "emulator.hh"

struct GCPadStatus *emulatorPadStatus;
void (*emulatorLoadState)(int slot);
void (*emulatorSaveState)(int slot, bool wait);
void (*emulatorFrameAdvance)(void);
void (*emulatorRunFunction)(uint32 address, uint32 stop);


struct processorState2 {
  uint32_t pc;
  uint32_t npc;
  uint32_t gpr[32];
  uint32_t lr;
};

namespace emulator {
  processorState ppcState;
  std::vector<processorState2> ppcStateStack;
  processorState2 savedState;
  
  auto pushProcessorState() -> void {
    processorState2 in;
    in.pc = *ppcState.pc;
    in.npc = *ppcState.npc;
    in.lr = *ppcState.lr;
    std::memcpy(in.gpr, ppcState.gpr, sizeof(uint32_t) * 32);
    savedState = in;
  }
    
  auto popProcessorState() -> void {
    processorState2 out = savedState;
    *ppcState.npc = out.npc;
    *ppcState.lr = out.lr;
    std::memcpy(ppcState.gpr, out.gpr, sizeof(uint32_t) * 32);
  }
    
  void (*createHook)(uint32_t, const char *, int, int, void (*)(void));
};
