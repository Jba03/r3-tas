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

#include "stream.h"
#include "engine.h"
#include "logging.h"
#include "gui.h"
#include "fix.h"
#include "lvl.h"
#include "math.h"
#include "memory.h"
#include "graphics.h"

#include <dispatch/dispatch.h>

//#include <SDL2/SDL.h>

#if defined(__APPLE__)
static dispatch_queue_t timer_queue;
static dispatch_source_t timer_source;
#endif

#pragma mark - Callbacks

void (*register_savestate_handler)(void (*handle)(int slot));
void (*register_loadstate_handler)(void (*handle)(int slot));
void (*register_render_callback)(void (*rc)(void* ctx));

#pragma mark - Main

void r3_load()
{
    log_indent = 0;
    
    if (graph_list) array_free(&graph_list);
    graph_list = array_create();
    
    uint32_t fixp = memory.read_32(memory.pointer_fix) & 0xFFFFFFF;
    if (fixp != 0x00)
    {
        info(BOLD COLOR_GREEN "Reading FIX @ %X\n", fixp);
        if (!(fix = fix_read(fixp)))
        {
            warning("Could not read FIX.\n");
            return;
        }
    }

    uint32_t lvlp = memory.read_32(memory.pointer_lvl) & 0xFFFFFFF;
    if (lvlp != 0x00)
    {
        info(BOLD COLOR_GREEN "Reading LVL @ %X\n", fixp);
        if (!(lvl = lvl_read(lvlp)))
        {
            warning("Could not read LVL.\n");
            return;
        }
    }
    
    /* Free previous hierarchy, if any */
    if (engine) superobject_free(&engine->root);

    info(BOLD "Reading engine struct");
    engine = engine_read(memory.pointer_engine);
    
    
    /* The game seems to alternate between two hierarchy pointers, that are
     * switched when loading a new level. Choose the currently valid pointer. */
    uint32_t entry1 = memory.read_32(memory.pointer_hierarchy1) & 0xFFFFFFF;
    uint32_t entry2 = memory.read_32(memory.pointer_hierarchy2) & 0xFFFFFFF;
    uint32_t hierarchy_entry = max(entry1, entry2); /* TODO: This address is important */
    hierarchy_entry = memory.read_32(hierarchy_entry + 4 * 5) & 0xFFFFFFF;

    if (hierarchy_entry == 0x00) return;

    info(BOLD COLOR_PINK "Reading superobject hierarchy @ %X\n", hierarchy_entry);
    clock_t start = clock();
    engine->root = superobject_read(hierarchy_entry);
    clock_t end = clock();

    const float dt = (float)(end - start) / (float)CLOCKS_PER_SEC;
    info(BOLD COLOR_PINK "Parsed superobject hierarchy in %.5f seconds\n", dt);
    
    info("num graphs: %d\n", array_element_count(graph_list));
}

static void load_check()
{
    uint8_t mode = memory.read_8(memory.pointer_engine);
    if (mode != 0)
    {
        info(BOLD "Loading (%d)...\n", mode);
        r3_load();
        dispatch_release(timer_source);
    }
}

static void loadstate_handler(int slot)
{
    info(BOLD COLOR_GREEN "Slot %d loaded\n", slot);
    r3_load();
    //engine_export_obj(engine);
}

static void savestate_handler(int slot)
{
    info(BOLD COLOR_GREEN "Slot %d saved\n", slot);
}

static void (*functions[]) =
{
    /* Read/write */
    &memory.read_8,
    &memory.read_16,
    &memory.read_32,
    &memory.read_float,
    &memory.write_8,
    &memory.write_16,
    &memory.write_32,
    &memory.write_float,
    
    /* Save / loadstates */
    &register_savestate_handler,
    &register_loadstate_handler,
    
    /* Rendering */
    &register_render_callback,
};

__attribute__((constructor)) void install(void)
{
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.25 * NSEC_PER_SEC)), dispatch_get_main_queue(),
    ^{
        FILE* fp = fopen("/tmp/r3tas_pointers", "rb");
        if (!fp)
        {
            error(BOLD "/tmp/r3tas_pointers not found - exiting.\n");
            exit(-1);
        }
        
        for (int i = 0; i < sizeof functions / sizeof *functions; i++)
        {
            void (*func)(void) = functions[i];
            fread(func, sizeof(uint64_t), 1, fp);
        }
        
        fclose(fp);
        
        info(BOLD "r3lib loaded successfully\n");
        info(FADED BOLD COLOR_BLUE "(read8" COLOR_GREY " = " COLOR_BLUE "%p)\n", &memory.read_8);
        info(FADED BOLD COLOR_BLUE "(read16" COLOR_GREY " = " COLOR_BLUE "%p)\n", &memory.read_16);
        info(FADED BOLD COLOR_BLUE "(read32" COLOR_GREY " = " COLOR_BLUE "%p)\n", &memory.read_32);
        info(FADED BOLD COLOR_BLUE "(readfloat" COLOR_GREY " = " COLOR_BLUE "%p)\n", &memory.read_float);
        info(FADED BOLD COLOR_BLUE "(write8" COLOR_GREY " = " COLOR_BLUE "%p)\n", &memory.write_8);
        info(FADED BOLD COLOR_BLUE "(write16" COLOR_GREY " = " COLOR_BLUE "%p)\n", &memory.write_16);
        info(FADED BOLD COLOR_BLUE "(write32" COLOR_GREY " = " COLOR_BLUE "%p)\n", &memory.write_32);
        info(FADED BOLD COLOR_BLUE "(writefloat" COLOR_GREY " = " COLOR_BLUE "%p)\n", &memory.write_float);
        
        info(FADED BOLD COLOR_BLUE "(register_savestate_handler" COLOR_GREY " = " COLOR_BLUE "%p)\n", &register_savestate_handler);
        info(FADED BOLD COLOR_BLUE "(register_loadstate_handler" COLOR_GREY " = " COLOR_BLUE "%p)\n", &register_loadstate_handler);
        info(FADED BOLD COLOR_BLUE "(register_render_callback" COLOR_GREY " = " COLOR_BLUE "%p)\n", &register_render_callback);
        info("\n");
        
        if (register_savestate_handler) register_savestate_handler(savestate_handler);
        if (register_loadstate_handler) register_loadstate_handler(loadstate_handler);
        if (register_render_callback) register_render_callback(render_callback);
        
        memory.pointer_fix = GCN_POINTER_FIX;
        memory.pointer_lvl = GCN_POINTER_LVL;
        memory.pointer_engine = GCN_POINTER_ENGINE;
        memory.pointer_hierarchy1 = GCN_POINTER_HIERARCHY1;
        memory.pointer_hierarchy2 = GCN_POINTER_HIERARCHY2;
        
        graphics_init();
    });
    
    timer_queue = dispatch_queue_create("load_timer", 0);
    timer_source = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, timer_queue);
    dispatch_source_set_event_handler(timer_source, ^{ load_check(); });
    dispatch_source_set_timer(timer_source, dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC), NSEC_PER_SEC * 0.5, 0);
    dispatch_resume(timer_source);
}
