//
//  emulator.h
//  r3-tas
//
//  Created by Jba03 on 2023-05-06.
//

#ifndef emulator_h
#define emulator_h

#include "game.hh"

#include <vector>

#define GAMEPAD_BUTTON_LEFT 0x0001
#define GAMEPAD_BUTTON_RIGHT 0x0002
#define GAMEPAD_BUTTON_DOWN 0x0004
#define GAMEPAD_BUTTON_UP 0x0008
#define GAMEPAD_TRIGGER_Z 0x0010
#define GAMEPAD_TRIGGER_R 0x0020
#define GAMEPAD_TRIGGER_L 0x0040
#define GAMEPAD_BUTTON_A 0x0100
#define GAMEPAD_BUTTON_B 0x0200
#define GAMEPAD_BUTTON_X 0x0400
#define GAMEPAD_BUTTON_Y 0x0800
#define GAMEPAD_BUTTON_START 0x1000

struct GCPadStatus {
  uint16 button = 0;       // Or-ed GAMEPAD_BUTTON_* and PAD_TRIGGER_* bits
  uint8 stickX = 0;        // 0 <= stickX       <= 255
  uint8 stickY = 0;        // 0 <= stickY       <= 255
  uint8 substickX = 0;     // 0 <= substickX    <= 255
  uint8 substickY = 0;     // 0 <= substickY    <= 255
  uint8 triggerLeft = 0;   // 0 <= triggerLeft  <= 255
  uint8 triggerRight = 0;  // 0 <= triggerRight <= 255
  uint8 analogA = 0;       // 0 <= analogA      <= 255
  uint8 analogB = 0;       // 0 <= analogB      <= 255
  bool isConnected = 0; // True
    
  uint8 MAIN_STICK_CENTER_X = 0x80; // 0x80;
  uint8 MAIN_STICK_CENTER_Y = 0x80; // 0x80;
  uint8 MAIN_STICK_RADIUS = 0x7F; // 0x7f;
  uint8 C_STICK_CENTER_X = 0x80; // 0x80;
  uint8 C_STICK_CENTER_Y = 0x80; // 0x80;
  uint8 C_STICK_RADIUS = 0x7F; // 0x7f;
    
  GCPadStatus() { }
};

extern struct GCPadStatus *emulatorPadStatus;
extern void (*emulatorLoadState)(int slot);
extern void (*emulatorSaveState)(int slot, bool wait);
extern void (*emulatorFrameAdvance)(void);
extern void (*emulatorRunFunction)(uint32 address, uint32 stop);

namespace emulator {
  
  struct message {
    int type;
    void *data;
  };
  
  struct processorState {
    uint32_t *pc = nullptr;
    uint32_t *npc = nullptr;
    uint32_t *gpr = nullptr;
    uint32_t *lr = nullptr;
  };
  
  /* CPU */
  extern processorState ppcState;
  void pushProcessorState();
  void popProcessorState();
  
  /// Hook create function
  extern void (*createHook)(uint32_t, const char *, int, int, void (*)(void));
};

#define PC     (* emulator::ppcState.pc     )
#define NPC    (* emulator::ppcState.npc    )
#define LR     (* emulator::ppcState.lr     )
#define GPR(i) (*(emulator::ppcState.gpr + i))

#endif /* emulator_h */
