//
//  main.c
//  r3lib
//
//  Created by Jba03 on 2022-11-12.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <dlfcn.h>

#include "mathc.h"
#include "log.h"
#include "game.h"
#include "structure.h"
#include "engine.h"
#include "superobject.h"
#include "actor.h"
#include "stdgame.h"
#include "gui.h"
#include "graphics.h"
#include "input.h"
//#include "vector3.h"
//#include "translate.h"

//#if defined(__APPLE__)
//#   include <mach/mach.h>
//#   include <mach/mach_vm.h>
//#   include <mach/vm_map.h>
//#endif

#pragma mark - Main

static uint8_t* mRAM = NULL;
static uint8_t* (*get_mRAM)(void) = NULL;
static bool ram_loaded = false;

bool* render_xfb_main;

void (*cpu_break)(void) = NULL;

#if defined(__APPLE__)
#include <dispatch/dispatch.h>
static dispatch_queue_t timer_queue;
static dispatch_source_t timer_source;
#endif

#define EXTERN_MESSAGE_INITIALIZE         0
#define EXTERN_MESSAGE_ON_UPDATE          1
#define EXTERN_MESSAGE_ON_VIDEO           2
#define EXTERN_MESSAGE_ON_SAVESTATE       3
#define EXTERN_MESSAGE_ON_LOADSTATE       4
#define EXTERN_MESSAGE_SAVESTATE_POINTER  5
#define EXTERN_MESSAGE_LOADSTATE_POINTER  6
#define EXTERN_MESSAGE_SAVESTATE_FUNCTION 7
#define EXTERN_MESSAGE_MRAM_POINTER       8


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
    
    hierarchy = (struct superobject*)(mRAM + hierarchy_entry);
    
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
}

static void r3_unload()
{
    array_free(&demo_save_names);
    array_free(&demo_level_names);
    array_free(&level_names);
    
    array_free(&family_names);
    array_free(&model_names);
    array_free(&instance_names);
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

static void update(const char* controller)
{
    if (!mRAM) memory.base = mRAM = get_mRAM();
    
//    input.joymain_y = 0;
//    input.joymain_x = 0;
//    input.joyc_y = 0;
//    input.joyc_x = 0;
//    input.a = false;
//    input.b = false;
//    input.x = false;
//    input.y = false;
//    input.z = false;
//    input.l = false;
//    input.r = false;
//    input.R = false;
//    input.S = false;
//
//    char button[128] = "", ignore[128] = "";
//    if (sscanf(controller, "P1: %[^C:]:%d,%d", button, &input.joymain_x, &input.joymain_y) == 3)
//        sscanf(controller + strlen(button) + 5, "%[^:]:%d,%d", ignore, &input.joyc_x, &input.joyc_y);
//    else
//        sscanf(controller + 4, "%sC:%d,%d", ignore, &input.joyc_x, &input.joyc_y);
//
//    char *str = button;
//    char *token = str;
//    while (token != NULL)
//    {
//        char* btn = strsep(&token, " ");
//        //printf("btn: %s\n", btn);
//        if (strcmp(btn, "A") == 0) input.a = true;
//        if (strcmp(btn, "B") == 0) input.b = true;
//        if (strcmp(btn, "X") == 0) input.x = true;
//        if (strcmp(btn, "Y") == 0) input.y = true;
//        if (strcmp(btn, "Z") == 0) input.z = true;
//        if (strcmp(btn, "L") == 0) input.l = true;
//        if (strcmp(btn, "R") == 0) input.r = true;
//        if (strcmp(btn, "RIGHT") == 0) input.R = true;
//        if (strcmp(btn, "START") == 0) input.S = true;
//    }
    
    /* Read input */
    {
        const struct input_entry* mjoyx = (const struct input_entry*)(memory.base + 0xB244C8);
        const struct input_entry* mjoyy = (const struct input_entry*)(memory.base + 0xB244FC);
        const struct input_entry* cjoyx = (const struct input_entry*)(memory.base + 0xB25300);
        const struct input_entry* cjoyy = (const struct input_entry*)(memory.base + 0xB25334);
        
        input.joymain_x = host_byteorder_f32(*(uint32_t*)&mjoyx->analogvalue);
        input.joymain_y = host_byteorder_f32(*(uint32_t*)&mjoyy->analogvalue);
        input.joyc_x = host_byteorder_f32(*(uint32_t*)&cjoyx->analogvalue);
        input.joyc_y = host_byteorder_f32(*(uint32_t*)&cjoyy->analogvalue);
    }
    
    /* Read global structures */
    engine = (struct engine*)(mRAM + GCN_POINTER_ENGINE);
    rnd = (struct rnd*)(mRAM + GCN_POINTER_RND);
    
    if just_entered_mode(6)
    {
        info(BOLD COLOR_PINK "Level transition began (frame %d)\n", engine->timer.frame);
        graphics_unload();
    }
    
    if just_entered_mode(9)
    {
        r3_load();
#ifdef OLD_VERSION
        graphics_load();
#endif
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
#ifdef OLD_VERSION
    *render_xfb_main = true;
#endif
    
    main_render = true;
    gui_render_callback(payload->context);
    
    //graphics_loop();
    
    //printf("swapfunc: %p\n", swapfunc);
    //if (swapfunc) swapfunc();
}

#pragma mark - Main

int on_load(void)
{
#ifdef OLD_VERSION
    graphics_init();
#endif
    
//    printf("sizeof default: %ul\n", sizeof(struct vector4));
//    printf("sizeof default: %ul\n", sizeof(struct vector44));
    
    info(BOLD "r3lib loaded successfully\n");
    return 1;
}

struct message
{
    int type;
    void* data;
};

#ifdef OLD_VERSION
void on_message(const char* message, void* data)
{
#define msg(type, code) if (!strcmp(message, type)) { code; }
    struct on_video_payload v;
    v.context = data;
    
    msg("ram_func", get_mRAM = data)
    //msg("xfb_texture", gui_render_game(data))
    msg("render_xfb_main", render_xfb_main = data)
    msg("cpu_break", cpu_break = data)
    msg("update", update(data))
    msg("render", video(&v))
#undef msg
}
#else
void on_message(struct message message)
{
    if (message.type == EXTERN_MESSAGE_ON_UPDATE) update("");
    if (message.type == EXTERN_MESSAGE_ON_VIDEO) video(message.data);
    if (message.type == EXTERN_MESSAGE_MRAM_POINTER) get_mRAM = message.data;
}
#endif

void on_unload(void)
{
}
