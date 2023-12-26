//
//  bruteforce.c
//  r3-tas
//
//  Created by Jba03 on 2023-06-26.
//

#include "bruteforce.hh"
#include "game.hh"
#include "structure.hh"
#include "memory.hh"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <dispatch/dispatch.h>

static bool bruteforce_on = false;
static unsigned int frameCounter = 0;
static unsigned int frameCounterLast = 0;
static unsigned int numStatesCurrent = 0;
static unsigned int numStatesAllocated = 0;
static struct State state[1000000];

struct State CurrentGameState()
{
    struct State S;
    /* Copy global superobject matrix */
    //S.globalMatrix = fnActorGet
    /* Copy dynamics state */
    S.dynamics = *fnActorGetDynamics(actor_rayman);
    //S.dynamics.base.report = 0;
    S.frame = host_byteorder_32(engine->timer.frame);
    S.score = vector3_length(vector3_sub(fnActorGetPosition(actor_rayman), fnActorGetPosition(actor_changemap)));
    
    return S;
}

struct State LastState()
{
    return state[numStatesCurrent-1];
}

void PushPlayerState(struct State S)
{
    state[numStatesCurrent++] = S;
}

void RestorePlayerState(struct State S)
{
    memcpy(fnActorGetDynamics(actor_rayman), &S.dynamics, sizeof S.dynamics);
}

static float lowest = INFINITY;

//struct State NewState()
//{
//
//}

static unsigned int framesToWait = 0;

static int frame = 0;


static long last = 0;

void BruteforceInput()
{
    //printf("hi2\n");
    //engine->timer.timerHandle = 0;
    //engine->inputMode = 1;
    //input.stick.main.x->analogValue = game_byteorder_f32(100);
    //printf("reset\n");
    //engine->timer.ticksPerMs = game_byteorder_32(40500);
    //engine->timer.frame = game_byteorder_32(host_byteorder_32(engine->timer.frame) + 1);
//    tdstInputStructure *st = (tdstInputStructure*)(memoryBase + 0x42F5BC);
//    tdstInputDevice *controller = &st->device[0];
//    controller->joypadCounter[0] = 1;
    
    //printf("before: %d\n", host_byteorder_32(engine->timer.totalRealTimeLow));
    
//    if (frame++ < 1)
//    {
//        input.stick.main.x->analogValue = game_byteorder_f32(100.0f);
//        //printf("Press\n");
//    }
//    else
//    {
//        //input.stick.main.x->analogValue = game_byteorder_f32(0.0f);
//    }
    
//    for (int i = 0; i < 16; i++)
//    {
//        engine->timer.counter[i] = game_byteorder_32(host_byteorder_32(engine->timer.counter[i]) + 1);
//    }
//
    //printf("hi!\n");
    
    //printf("a %d\n", frame);
//    if (frame++ == 2 && input.stick.main.x)
//    {
//        input.stick.main.x->analogValue = game_byteorder_f32(100.0f);
//        printf("Press\n");
//    }
//    else if (input.stick.main.x && frame > 6)
//    {
//        //input.stick.main.x->analogValue = game_byteorder_f32(0.0f);
//    }
    
    
//    else if (input.stick.main.x && frame == 14)
//    {
//        input.stick.main.x->analogValue = game_byteorder_f32(0.0f);
//    }
    
    //printf("hi!\n");
    
    
//    if (input.stick.main.x){
//        input.stick.main.x->analogValue = game_byteorder_f32(drand48() * 200.0f - 100.0f);
//        input.stick.main.y->analogValue = game_byteorder_f32(drand48() * 200.0f - 100.0f);
//    }
    
    //engine->timer.frame = game_byteorder_32(host_byteorder_32(engine->timer.frame) + 1);
    
    //engine->timer.totalRealTimeLow = game_byteorder_32(0);
    //printf("last: %d\n", last);
    
    printf("engine start hook\n");
}

int i = 0;

static bool requestLoadstate = false;

static uint8 *savestate = NULL;
static void FastLoadState()
{
    //memcpy(memoryBase, savestate, sizeof(uint8) * 24 * 1000 * 500);
}

void BruteforceMain(int *numLoops)
{
    *numLoops = 0;
    
    printf("engine end hook\n");
//
//        if (bruteforce_on)
//        {
//            //memset(emulatorPadStatus, 0, sizeof (struct GCPadStatus));
//
//
//            //RestorePlayerState(LastState());
//
//            //dispatch_sync(dispatch_get_global_queue(<#intptr_t identifier#>, <#uintptr_t flags#>), <#^(void)block#>)
//            //emulatorPadStatus->button = rand() % UINT16_MAX;
//
////            if (frameCounter < frameCounterLast)
////            {
////                printf("playing back frame %d / %d\n", frameCounter, frameCounterLast);
////                *emulatorPadStatus = state[frameCounter].gamepad;
////
////                //emulatorPadStatus->stickX = arc4random() % 255;
////                //emulatorPadStatus->stickY = arc4random() % 255;
////                //if ()
////
////                frameCounter++;
////            }
////            else
//            {
//                //frameCounter = 0;
//
//                /* Set the gamepad to a random state */
//                emulatorPadStatus->stickX = arc4random() % 255;
//                emulatorPadStatus->stickY = arc4random() % 255;
//                /* Wait one frame until inputs go through */
//                framesToWait = 3;
//
//                printf("Press: %d\n", host_byteorder_32(engine->timer.frame));
//
//
//
////                emulatorPadStatus->substickX = rand() % 255;
////                emulatorPadStatus->substickY = rand() % 255;
//
//                //if ((frameCounter % 10) == 0) emulatorLoadState(1);
//
//
//                //PushPlayerState(S);
//
////                tdstEngineObject *rayman = actor_rayman;
////                tdstEngineObject *endMap = actor_changemap;
////
////                float d = vector3_length(vector3_sub(fnActorGetPosition(rayman), fnActorGetPosition(endMap)));
////                //float t = (float)host_byteorder_32(engine->timer.frame) / d;
////                if (d < state[frameCounter].score)
////                {
////
////                    lowest = d;
////
////                    struct State S = CurrentGameState();
////                    S.gamepad = *emulatorPadStatus;
////                    PushPlayerState(S);
////
////                    frameCounter=0;
////                    //frameCounterLast++;
////                    frameCounterLast++;
////                    emulatorLoadState(1);
////                    //emulatorLoadState(1);
////                    //emulatorSaveState(1, true);
////                }
////                else
////                {
////                    //emulatorLoadState(1);
////                }
//
//
//                //input.stick.main.x->analogValue = game_byteorder_f32(-100.0f);
//
//
//                //tdstVector3D p = fnActorGetPosition(rayman);
//                //printf("pos: %.2f %.2f %.2f\n", p.x, p.y, p.z);
//            }
//
//            //frameCounter++;
//        }
//        else
//        {
//            lowest = vector3_length(vector3_sub(fnActorGetPosition(actor_rayman), fnActorGetPosition(actor_changemap)));
//        }
    
}


static enum StateMachineState
{
    S_WAIT,
    S_INPUT,
    S_INPUT_REGISTER,
    S_FIND_FIRST_FRAME,
    S_SEEK_FIRST_FRAME,
} CurrentState = S_FIND_FIRST_FRAME, NextState;

static int WaitDuration = 0;
static unsigned LastFrameHandled = 0;
static float BestDistance = 0.0f;

static void RandomInput(struct GCPadStatus *gamepad)
{
    bool a = rand() % 10;
    bool b = rand() % 10;
    
    if (a) gamepad->button |= GAMEPAD_BUTTON_A;
    if (b) gamepad->button |= GAMEPAD_BUTTON_B;
    gamepad->stickX = rand() % 255;
    gamepad->stickY = rand() % 255;
}

void BruteforceStateHandler(struct GCPadStatus *gamepad)
{
    
begin:
    
    //printf("hi!\n");
    if (hierarchy)
    {
        engine->timer.ticksPerMs = game_byteorder_32(40500/100);
        
        /* Only the last polled input is ever handled */
        if (host_byteorder_32(engine->timer.frame) == LastFrameHandled)
        {
            switch (CurrentState)
            {
                case S_WAIT:
                {
                    WaitDuration--;
                    if (WaitDuration-1 <= 0)
                    {
                        CurrentState = NextState;
                    }
                    printf("wait: %d\n", WaitDuration);
                    break;
                }
                    
                case S_INPUT_REGISTER:
                {
                    float d = vector3_length(vector3_sub(fnActorGetPosition(actor_rayman), fnActorGetPosition(actor_changemap)));
                    
                    if (d < BestDistance)
                    {
                        emulatorLoadState(1);
                        printf("best reached: %f\n", d);
                        BestDistance = d;
                        
                        CurrentState = S_INPUT;
                        goto begin;
                    }
                    
                    break;
                }
                
                case S_INPUT:
                {
                    RandomInput(gamepad);
                    
                    WaitDuration = 1;
                    CurrentState = S_WAIT;
                    NextState = S_INPUT_REGISTER;
                    break;
                }
                    
                case S_FIND_FIRST_FRAME: // Find first available input frame
                {
                    //printf("%s\n", fnActorGe(actor_rayman));
                    char* S = fnActorGetCurrentBehaviorName(actor_rayman);
                    if (S) S = strchr(S, ':') + 1;
                    
                    /* All buttons available here */
    //                if (strcmp("YAM_C_Attente", S) == 0)
    //                {
    //                    printf("save!\n");
    //                    //gamepad->stickX = 255;
    //                    /* Save state and return S_WAITINg */
    //                    emulatorSaveState(1, false);
    //                    CurrentState = S_INPUT;
    //                }
                    
                    /* Strafe lock available on YAM_C_Init */
                    if (strcmp("YAM_C_Init", S) == 0)
                    {
                        // TODO: Determine if strafe locking should be done for faster initial turn angle
                        //gamepad->button = GAMEPAD_TRIGGER_R;
                        
                        BestDistance = vector3_length(vector3_sub(fnActorGetPosition(actor_rayman), fnActorGetPosition(actor_changemap)));
                        
                        CurrentState = S_INPUT;
                        
                        emulatorSaveState(1, true);
                    }
                    
                    break;
                }
                    
                case S_SEEK_FIRST_FRAME:
                {
                    
                }
                    
                default:
                    break;
            }
        }
        
        LastFrameHandled = host_byteorder_32(engine->timer.frame);
        
    }
    
    //printf("hi!\n");
    
//    srand(time(0));
    //gamepad->stickX = drand48() * 255;
    //gamepad->stickY = drand48() * 255;
//
//    if (frameCounter++ >= 3)
//    {
//        if (frameCounter == 4)
//        {
//            float d = vector3_length(vector3_sub(fnActorGetPosition(actor_rayman), fnActorGetPosition(actor_changemap)));
//            printf("d: %f\n", d);
//        }
//        else if (frameCounter == 6)
//        {
//            //FastLoadState();
//            emulatorLoadState(1);
//            frameCounter = 0;
//
//
//        }
//
//       // FastLoadState();
//    }
    
//    if (requestLoadstate)
//    {
//        emulatorLoadState(1);
//        requestLoadstate = false;
//    }
    
//    if (input.stick.main.x) {
//        input.stick.main.x->analogValue = game_byteorder_f32(drand48() * 200.0f - 100.0f);
//        input.stick.main.y->analogValue = game_byteorder_f32(drand48() * 200.0f - 100.0f);
//    }
}


void BruteforceBegin()
{
    // 803e7ccc
    
    emulatorSaveState(1, true);
    
    //last = host_byteorder_32(engine->timer.totalRealTimeLow);
    
    //engine->timer.ticksPerMs = game_byteorder_32(40500 / 10);
    
   // srand(time(0));
    frameCounter = 0;
    //savestate = malloc(sizeof(uint8) * 24 * 1000 * 500);
    //memcpy(savestate, memoryBase, sizeof(uint8) * 24 * 1000 * 500);
    //PushPlayerState(CurrentGameState());
    bruteforce_on = true;
}

void BeforeEngineExecuted(int *numLoops)
{
    //engine->inputMode=1;
    //*numLoops = 0;
    
    //engine->timer.ticksPerMs = game_byteorder_32(40500/2);

//    if (last == 0)
//    {
//        last = host_byteorder_32(engine->timer.totalRealTimeLow);
//    }
//    else
//    {
//        long diff = host_byteorder_32(engine->timer.totalRealTimeLow) - last;
//        //printf("diff: %ld\n", diff);
//        engine->timer.totalRealTimeLow = game_byteorder_32(last + diff*2);
//    }
//
//    last = host_byteorder_32(engine->timer.totalRealTimeLow);
//
//
//    //printf("Before engine execution\n");
    if (frame < 1)
    {
        input.stick.main.x->analogValue = game_byteorder_f32(/*drand48() * 200.0f - 100.0f*/100.0f);
        //input.stick.main.y->analogValue = game_byteorder_f32(drand48() * 200.0f - 100.0f);
        //printf("press\n");
    }

    frame++;
//
//    printf("input: %d\n", input.stick.main.x->analogValue);
}

void AfterEngineExecuted()
{
    
    //printf("After engine execution\n");
    
    input.stick.main.x->analogValue = 0;
}

static int aaaf = 0;

static bool cycle()
{
    aaaf++;
    if ((aaaf % 50) == 0) return false;
    return true;
}


const struct BruteforcingTool * bruteforcer = &(const struct BruteforcingTool)
{
    &cycle,
};
