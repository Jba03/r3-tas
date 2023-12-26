//
//  main.c
//  r3lib
//
//  Created by Jba03 on 2022-11-12.
//

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#include "game.hh"
#include "log.hh"
#include "mathc.hh"
#include "memory.hh"
#include "bruteforce.hh"

#include "stEngineStructure.hh"
#include "stSuperObject.hh"
#include "stEngineObject.hh"
#include "stStandardGameInfo.hh"
#include "gui.hh"
#include "graphics.hh"
#include "stInputStructure.hh"
#include "emulator.hh"

#pragma mark - Main

static uint8_t* mRAM = NULL;
static uint8_t* (*get_mRAM)(void) = NULL;
const char* (*get_config_path)(void) = NULL;

struct message
{
    int type;
    void* data;
};


#define H1(s,i,x)   (x*65599u+(uint8_t)s[(i)<strlen(s)?strlen(s)-1-(i):strlen(s)])
#define H4(s,i,x)   H1(s,i,H1(s,i+1,H1(s,i+2,H1(s,i+3,x))))
#define H16(s,i,x)  H4(s,i,H4(s,i+4,H4(s,i+8,H4(s,i+12,x))))
#define H64(s,i,x)  H16(s,i,H16(s,i+16,H16(s,i+32,H16(s,i+48,x))))
#define H256(s,i,x) H64(s,i,H64(s,i+64,H64(s,i+128,H64(s,i+192,x))))
#define m(x)        ((uint32_t)(H256(x,0,0)^(H256(x,0,0)>>16)))

#if defined(WIN32)
#   define LIBR3TAS_EXPORT __declspec(dllexport)
#else
#   define LIBR3TAS_EXPORT
#endif

static void r3_load()
{
    info(BOLD "\nLoading...\n");
    
    level_read();
    
    hierarchy               = (tdstSuperObject*)pointer(*(uint32*)(mRAM + GCN_POINTER_ACTUAL_WORLD));
    dynamic_world           = (tdstSuperObject*)pointer(*(uint32*)(mRAM + GCN_POINTER_DYNAMIC_WORLD));
    inactive_dynamic_world  = (tdstSuperObject*)pointer(*(uint32*)(mRAM + GCN_POINTER_INACTIVE_DYNAMIC_WORLD));
    father_sector           = (tdstSuperObject*)pointer(*(uint32*)(mRAM + GCN_POINTER_FATHER_SECTOR));
    
    /* Find some actors */
    actor_rayman = fnFindActor(actor_instance_name, "Rayman", hierarchy);
    actor_camera = fnFindActor(actor_instance_name, "StdCamer", hierarchy);
    actor_global = fnFindActor(actor_instance_name, "global", hierarchy);
    actor_world  = fnFindActor(actor_instance_name, "World", hierarchy);
    actor_changemap = fnFindActor(actor_model_name, "NIN_m_ChangeMap", hierarchy);
    
    info(BOLD COLOR_GREEN "HIE @ [0x%X : %p]\n\n", offset(hierarchy), hierarchy);
    
    if (engine) strcpy(previous_level_name, engine->currentLevelName);
    
    //BruteforceBegin();
    
//    int nLoops = 100;
//    engine->timer.ticksPerMs = game_byteorder_32(40500 / nLoops);
    //engine->timer.frameLength = game_byteorder_f32(1.0f / 60.0f);
}

static void r3_unload()
{
    game_unload();
    
    hierarchy = NULL;
    dynamic_world = NULL;
    inactive_dynamic_world = NULL;
    father_sector = NULL;
}

static void load()
{
    #pragma mark Input structure
    {
        unsigned long io = offsetof(tdstInputStructure, entries);
        IPT_g_hInputStructure = (tdstInputStructure*)(memoryBase + GCN_POINTER_INPUT - io);
        printf("IPT @ %X\n", offset(IPT_g_hInputStructure));
        
        input.stick.main.x = fnInputEntryFind(IPT_g_hInputStructure, "Action_Pad0_AxeX");
        input.stick.main.y = fnInputEntryFind(IPT_g_hInputStructure, "Action_Pad0_AxeY");
        input.stick.c.x    = fnInputEntryFind(IPT_g_hInputStructure, "Action_Pad0_AxeV");
        input.stick.c.y    = fnInputEntryFind(IPT_g_hInputStructure, "Action_Pad0_AxeZ");
        
        input.button.a = fnInputEntryFind(IPT_g_hInputStructure, "Action_Sauter");
        input.button.b = fnInputEntryFind(IPT_g_hInputStructure, "Action_Affiche_Jauge");
        input.button.x = fnInputEntryFind(IPT_g_hInputStructure, "Action_Tirer");
        input.button.y = fnInputEntryFind(IPT_g_hInputStructure, "Action_Camera_Look");
        input.button.S = fnInputEntryFind(IPT_g_hInputStructure, "Button_Start");
        input.button.R = fnInputEntryFind(IPT_g_hInputStructure, "Action_Strafe");
        input.button.L = fnInputEntryFind(IPT_g_hInputStructure, "Action_Baisser");
        
        input.button.l = fnInputEntryFind(IPT_g_hInputStructure, "Action_Camera_TourneDroite");
    }
    
    r3_unload();
    r3_load();
}

static void loadstate_handler(int slot)
{
    //r3_unload();
    info(BOLD COLOR_GREEN "Slot %d loaded\n", slot);
    bool level_changed = strcmp(previous_level_name, engine->currentLevelName) != 0;
    if (level_changed && engine->mode == 9) load();
}

static void savestate_handler(int slot)
{
    info(BOLD COLOR_GREEN "Slot %d saved\n", slot);
}

static void update(void *data)
{
    if (!mRAM) memoryBase = mRAM = get_mRAM();
    
    //printf("got so far\n");
    
    /* Read global structures */
    engine = (tdstEngineStructure*)(mRAM + GCN_POINTER_ENGINE);
    rnd = (tdstRandom*)(mRAM + GCN_POINTER_RND);
    
    if just_entered_mode(6)
    {
        info(BOLD COLOR_PINK "Level transition began (frame %d)\n", engine->timer.frame);
        
        r3_unload();
    }
    
    if just_entered_mode(9)
    {
        r3_unload();
        load();
    }
    
    if just_entered_mode(5)
        transition_frame = 0;
        
    switch (engine->mode)
    {
        case 0: /* Invalid mode */
            break;

        case 1: /* Initialize engine */
            break;

        case 2: /* Deinitialize engine */
            break;

        case 3: /* Initialize game loop */
            break;

        case 4: /* Deinitialize game loop */
            break;

        case 5: /* Enter level */
            transition_frame++;
            break;

        case 6: /* Change level */
            info(BOLD COLOR_PINK "Level transition began (frame %d)\n", engine->timer.frame);
            break;

        case 7: /* Dead loop */
            break;

        case 8: /* Player dead */
            break;

        case 9: /* Normal gameplay */
            break;

    }

    previous_engine_mode = engine->mode;
}

bool main_render = true;

struct on_video_payload
{
    void* texture;
    void* context;
    bool windowed;
};

static void video(struct message *msg)
{
    main_render = false;
    struct on_video_payload* payload = msg->data;
    gui_render_callback(payload->context, payload->texture, &payload->windowed);
}

#pragma mark - Main

static void onload(struct message *msg)
{
   // graphics_init();
    gui_init();
    
    
    memset(previous_level_name, 0, 30);
    
    info(BOLD "r3-tas loaded successfully\n");
    
    /* Return success. */
    *(int*)(msg->data) = 0;
}

static void receive_mram_function(struct message *msg)
{
    get_mRAM = msg->data;
}

static void create_hle_hooks(struct message *msg)
{
    extern void hle_hook_functions(void *);
    hle_hook_functions(msg->data);
}

static void receive_ppc_state(struct message *msg)
{
    extern void ppc_state_receive(void *);
    ppc_state_receive(msg->data);
}

static void unload(struct message *msg)
{
    info(BOLD COLOR_RED "r3-tas successfully unloaded\n");
}

LIBR3TAS_EXPORT void on_message(struct message msg)
{
    switch (msg.type)
    {
        case m("load"):
            onload(&msg);
            break;
            
        case m("mram-function"):
            receive_mram_function(&msg);
            break;
            
        case m("update"):
            update(&msg);
            break;
            
        case m("hle-hook"):
            create_hle_hooks(&msg);
            break;
            
        case m("ppcstate"):
            receive_ppc_state(&msg);
            break;
            
        case m("input-request"):
            break;
            
        case m("video"):
            video(&msg);
            break;
            
        case m("unload"):
            unload(&msg);
            break;
    }
    
//    if (message.type == EXTERN_MESSAGE_ON_UPDATE) update("");
//    if (message.type == EXTERN_MESSAGE_ON_VIDEO) video(message.data);
//    if (message.type == EXTERN_MESSAGE_ON_LOADSTATE) loadstate_handler(*(int*)message.data);
//    if (message.type == EXTERN_MESSAGE_ON_SAVESTATE) savestate_handler(*(int*)message.data);
//    if (message.type == EXTERN_MESSAGE_LOADSTATE_FUNCTION) emulatorLoadState = message.data;
//    if (message.type == EXTERN_MESSAGE_SAVESTATE_FUNCTION) emulatorSaveState = message.data;
    //if (message.type == EXTERN_MESSAGE_MRAM_POINTER) get_mRAM = message.data;
//    if (message.type == EXTERN_MESSAGE_CONFIG_POINTER) get_config_path = message.data;
//    if (message.type == EXTERN_MESSAGE_SET_INPUT_STATUS) {
//        BruteforceStateHandler(message.data);
//        //BruteforceInput();
//        //emulatorPadStatus = message.data;
//        //BruteforceMain();
//        //printf("request pad status: %p\n", message.data);
//    }
//   // if (message.type == EXTERN_MESSAGE_FRAMEADVANCE_FUNCTION) emulatorFrameAdvance = message.data;
//
//    if (message.type == 11)
//    {
//        emulatorRunFunction = message.data;
//    }
//
//    if (message.type == 12)
//    {
//        AfterEngineExecuted();
//        //BruteforceMain(message.data);
//    }
//
//    if (message.type == 23)
//    {
//        //printf("hi!\n");
//        BeforeEngineExecuted(message.data);
//        //BruteforceInput();
//    }
}

LIBR3TAS_EXPORT void on_unload(void)
{
}
