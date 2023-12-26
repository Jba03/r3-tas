//
//  bruteforce.h
//  r3-tas
//
//  Created by Jba03 on 2023-06-26.
//

#ifndef bruteforce_h
#define bruteforce_h

#include "stDynamics.hh"
#include "emulator.hh"

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

extern const struct BruteforcingTool *bruteforcer;

struct State CurrentGameState();

struct State LastState();

void PushPlayerState(struct State S);

void RestorePlayerState(struct State S);

void BruteforceBegin();

void BruteforceInput();

void BruteforceStateHandler(struct GCPadStatus *gamepad);

void BruteforceMain(int *numLoops);

void BeforeEngineExecuted(int *numLoops);
void AfterEngineExecuted();

#endif /* bruteforce_h */
