//
//  game.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-19.
//

#ifndef game_h
#define game_h

#include <stdint.h>

#include "stEngineStructure.h"
#include "stSuperobject.h"
#include "stEngineObject.h"
#include "stRandom.h"
#include "structure.h"
#include "stInputStructure.h"
#include "array.h"
#include "route.h"

#define GCN_POINTER_ENGINE      0x003E7C0C
#define GCN_POINTER_INPUT       0x00430904
#define GCN_POINTER_FIX         0x004334cc
#define GCN_POINTER_LVL         0x004334d0
#define GCN_POINTER_HIERARCHY1  0x004334d4
#define GCN_POINTER_HIERARCHY2  0x004334d8
#define GCN_POINTER_RND         0x00436924
#define GCN_POINTER_CINEMANAGER 0x003E9760

extern struct stEngineStructure* engine;
extern struct stSuperObject* hierarchy;
extern struct stSuperObject* dynamic_world;
extern struct stSuperObject* inactive_dynamic_world;
extern struct stSuperObject* father_sector;
extern struct stInputStructure* input_struct;
extern struct stRandom* rnd;
extern struct stAlways* alwaysStructure;
extern struct stObjectType* objectType;

extern tdstEngineObject* actor_rayman;
extern tdstEngineObject* actor_camera;
extern tdstEngineObject* actor_global;
extern tdstEngineObject* actor_world;
extern tdstEngineObject* actor_changemap;

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
        struct { tdstInputEntryElement *x, *y; } main;
        struct { tdstInputEntryElement *x, *y; } c;
    } stick;
    
    struct
    {
        tdstInputEntryElement* a;
        tdstInputEntryElement* b;
        tdstInputEntryElement* x;
        tdstInputEntryElement* y;
        tdstInputEntryElement* z;
        tdstInputEntryElement* l;
        tdstInputEntryElement* r;
        tdstInputEntryElement* S;
        tdstInputEntryElement* L;
        tdstInputEntryElement* U;
        tdstInputEntryElement* D;
        tdstInputEntryElement* R;
    } button;
} input;

extern struct route routes[16];
extern unsigned int current_route;
extern unsigned int n_routes;

void level_read(void);

void game_unload(void);

uint32 color_table_index(unsigned idx);

uint32 actor_color(const tdstEngineObject* actor);

void game_memory_dump(void);

#endif /* game_h */
