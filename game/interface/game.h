//
//  game.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-19.
//

#ifndef game_h
#define game_h

#include <stdint.h>

struct memory { const uint8_t* base, *tmp; } extern memory;

#include "structure.h"
#include "array.h"
#include "input.h"
#include "fix.h"
#include "lvl.h"

#define GCN_POINTER_ENGINE      0x003E7C0C
#define GCN_POINTER_INPUT       0x00430904
#define GCN_POINTER_FIX         0x004334cc
#define GCN_POINTER_LVL         0x004334d0
#define GCN_POINTER_HIERARCHY1  0x004334d4
#define GCN_POINTER_HIERARCHY2  0x004334d8
#define GCN_POINTER_RND         0x00436924
#define GCN_POINTER_CINEMANAGER 0x003E9760

extern struct engine* engine;
extern struct superobject* hierarchy;
extern struct superobject* dynamic_world;
extern struct superobject* inactive_dynamic_world;
extern struct superobject* father_sector;
extern struct input_structure* input_struct;
extern struct rnd* rnd;
extern struct fix fix;
extern struct lvl lvl;

extern struct actor* actor_rayman;
extern struct actor* actor_camera;
extern struct actor* actor_global;
extern struct actor* actor_world;
extern struct actor* actor_changemap;

/* */
extern uint8_t previous_engine_mode;
extern char previous_level_name[30];
extern unsigned transition_frame;
/* just_entered_mode: true if engine mode differs from previous frame */
#define just_entered_mode(m) ((engine->mode == m) && (previous_engine_mode != m))

/* TAS */
extern struct inputstructure
{
    struct
    {
        struct { struct input_entry *x, *y; } main;
        struct { struct input_entry *x, *y; } c;
    } stick;
    
    struct
    {
        struct input_entry* a;
        struct input_entry* b;
        struct input_entry* x;
        struct input_entry* y;
        struct input_entry* z;
        struct input_entry* l;
        struct input_entry* r;
        struct input_entry* S;
        struct input_entry* L;
        struct input_entry* U;
        struct input_entry* D;
        struct input_entry* R;
    } button;
} input;

void level_read(void);

uint32 color_table_index(unsigned idx);

uint32 actor_color(const struct actor* actor);

/* Structure functions */
#include "find.h"

#endif /* game_h */
