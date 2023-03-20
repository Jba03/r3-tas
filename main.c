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

#include "mathc.h"
#include "log.h"
#include "game.h"
#include "structure.h"
#include "stEngineStructure.h"
#include "stSuperObject.h"
#include "stEngineObject.h"
#include "stStandardGameInfo.h"
#include "gui.h"
#include "graphics.h"
#include "stInputStructure.h"

#pragma mark - Main

static uint8_t* mRAM = NULL;
static uint8_t* (*get_mRAM)(void) = NULL;
const char* (*get_config_path)(void) = NULL;

#define EXTERN_MESSAGE_INITIALIZE         0
#define EXTERN_MESSAGE_ON_UPDATE          1
#define EXTERN_MESSAGE_ON_VIDEO           2
#define EXTERN_MESSAGE_ON_SAVESTATE       3
#define EXTERN_MESSAGE_ON_LOADSTATE       4
#define EXTERN_MESSAGE_SAVESTATE_POINTER  5
#define EXTERN_MESSAGE_LOADSTATE_POINTER  6
#define EXTERN_MESSAGE_SAVESTATE_FUNCTION 7
#define EXTERN_MESSAGE_MRAM_POINTER       8
#define EXTERN_MESSAGE_CONFIG_POINTER     9

#if defined(WIN32)
#   define LIBR3TAS_EXPORT __declspec(dllexport)
#else
#   define LIBR3TAS_EXPORT
#endif

static void r3_load()
{
    info(BOLD "\nLoading...\n");
    
    /* Hmm, does this influence the game? */
    //srand(time(NULL));
    
    level_read();
    
    /* The game seems to alternate between two hierarchy pointers, that are
     * switched when loading a new level. Choose the currently valid pointer. */
    uint32_t entry1 = host_byteorder_32(*(uint32_t*)(mRAM + GCN_POINTER_HIERARCHY1)) & 0xFFFFFFF;
    uint32_t entry2 = host_byteorder_32(*(uint32_t*)(mRAM + GCN_POINTER_HIERARCHY2)) & 0xFFFFFFF;
    uint32_t hierarchy_entry = max(entry1, entry2); /* Weird structure at this address. */
    hierarchy_entry = host_byteorder_32(*(uint32_t*)(mRAM + hierarchy_entry + 4 * 5)) & 0xFFFFFFF;
    
    hierarchy = (tdstSuperObject*)(mRAM + hierarchy_entry);
    
    info(BOLD COLOR_GREEN "HIE @ [0x%X : %p]\n\n", hierarchy_entry, hierarchy);
    
    /* Find some actors */
    actor_rayman = actor_find(actor_instance_name, "Rayman", hierarchy);
    actor_camera = actor_find(actor_instance_name, "StdCamer", hierarchy);
    actor_global = actor_find(actor_instance_name, "global", hierarchy);
    actor_world  = actor_find(actor_instance_name, "World", hierarchy);
    actor_changemap = actor_find(actor_model_name, "NIN_m_ChangeMap", hierarchy);
    
    info(COLOR_BLUE "Rayman @ %X\n", offset(actor_rayman));
    info(COLOR_BLUE "StdCamer @ %X\n", offset(actor_camera));
    info(COLOR_BLUE "global @ %X\n", offset(actor_global));
    info(COLOR_BLUE "World @ %X\n", offset(actor_world));
    info(COLOR_BLUE "NIN_m_ChangeMap @ %X\n", offset(actor_changemap));
    
    /* Derive the three worlds from the hierarchy */
    if (hierarchy) dynamic_world = (tdstSuperObject*)pointer(hierarchy->first_child);
    if (dynamic_world) inactive_dynamic_world = (tdstSuperObject*)pointer(dynamic_world->next);
    if (inactive_dynamic_world) father_sector = (tdstSuperObject*)pointer(hierarchy->last_child);
    
    if (engine) strcpy(previous_level_name, engine->current_level_name);
}

static void r3_unload()
{
    game_unload();
    
    hierarchy = NULL;
    dynamic_world = NULL;
    inactive_dynamic_world = NULL;
    father_sector = NULL;
}

//static void loadstate_handler(int slot)
//{
//    info(BOLD COLOR_GREEN "Slot %d loaded\n", slot);
//    r3_load();
//}
//
//static void savestate_handler(int slot)
//{
//    info(BOLD COLOR_GREEN "Slot %d saved\n", slot);
//}

static void load()
{
    #pragma mark Input structure
    {
        unsigned long io = offsetof(tdstInputStructure, entries);
        input_struct = (tdstInputStructure*)(memory.base + GCN_POINTER_INPUT - io);
        
        input.stick.main.x = input_entry_find(input_struct, "Action_Pad0_AxeX");
        input.stick.main.y = input_entry_find(input_struct, "Action_Pad0_AxeY");
        input.stick.c.x    = input_entry_find(input_struct, "Action_Pad0_AxeV");
        input.stick.c.y    = input_entry_find(input_struct, "Action_Pad0_AxeZ");
        
        input.button.a = input_entry_find(input_struct, "Action_Sauter");
        input.button.b = input_entry_find(input_struct, "Action_Affiche_Jauge");
        input.button.x = input_entry_find(input_struct, "Action_Tirer");
        input.button.y = input_entry_find(input_struct, "Action_Camera_Look");
        input.button.S = input_entry_find(input_struct, "Button_Start");
        input.button.R = input_entry_find(input_struct, "Action_Strafe");
        input.button.L = input_entry_find(input_struct, "Action_Baisser");
        
        input.button.l = input_entry_find(input_struct, "Action_Camera_TourneDroite");
    }
    
    r3_unload();
    r3_load();
    
#ifdef OLD_VERSION
    //graphics_load();
#endif
}

static void update(const char* controller)
{
    if (!mRAM) memory.base = mRAM = get_mRAM();
    
    /* Read global structures */
    engine = (tdstEngineStructure*)(mRAM + GCN_POINTER_ENGINE);
    rnd = (tdstRandom*)(mRAM + GCN_POINTER_RND);
    
    if just_entered_mode(6)
    {
        info(BOLD COLOR_PINK "Level transition began (frame %d)\n", engine->timer.frame);
        
        r3_unload();
#ifdef OLD_VERSION
        //graphics_unload();
#endif
    }

    if just_entered_mode(9)
    {
        r3_unload();
        load();
    }
    else if (previous_level_name[0] != '\0' && engine->mode == 9)
    {
        if (strcmp(previous_level_name, engine->current_level_name))
        {
            r3_unload();
            load();
        }
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
            info(BOLD COLOR_PINK "Level transition began (frame %d)", engine->timer.frame);
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
    bool draw;
};

static void video(struct on_video_payload* payload)
{
    
    //graphics_loop();
    
    main_render = true;
    gui_render_callback(payload->context);
    
    //gui_render_game(payload->texture);
    
    //graphics_loop();
    
    //printf("swapfunc: %p\n", swapfunc);
    //if (swapfunc) swapfunc();
}

#pragma mark - Main

LIBR3TAS_EXPORT int on_load(void)
{
#ifdef OLD_VERSION
    //graphics_init();
#endif
    
    memset(previous_level_name, 0, 30);
    
    info(BOLD "r3lib loaded successfully\n");
    return 1;
}

struct message
{
    int type;
    void* data;
};

#ifdef OLD_VERSION
LIBR3TAS_EXPORT void on_message(const char* message, void* data)
{
#define msg(type, code) if (!strcmp(message, type)) { code; }
    struct on_video_payload v;
    v.context = data;
    
    msg("ram_func", get_mRAM = data)
    //msg("xfb_texture", gui_render_game(data))
    msg("update", update(data))
    msg("render", video(&v))
#undef msg
}
#else
LIBR3TAS_EXPORT void on_message(struct message message)
{
    if (message.type == EXTERN_MESSAGE_ON_UPDATE) update("");
    if (message.type == EXTERN_MESSAGE_ON_VIDEO) video(message.data);
    if (message.type == EXTERN_MESSAGE_MRAM_POINTER) get_mRAM = message.data;
    if (message.type == EXTERN_MESSAGE_CONFIG_POINTER) get_config_path = message.data;
}
#endif

LIBR3TAS_EXPORT void on_unload(void)
{
}
