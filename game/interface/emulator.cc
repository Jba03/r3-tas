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
