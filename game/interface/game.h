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

#define GCN_POINTER_ENGINE      0x003E7C0C
#define GCN_POINTER_FIX         0x004334cc
#define GCN_POINTER_LVL         0x004334d0
#define GCN_POINTER_HIERARCHY1  0x004334d4
#define GCN_POINTER_HIERARCHY2  0x004334d8
#define GCN_POINTER_RND         0x00436924
#define GCN_POINTER_CINEMANAGER 0x0134b4c8

/* actor_find */
#define actor_family_name   0
#define actor_model_name    1
#define actor_instance_name 2

/* Global structures */
extern struct engine* engine;
extern struct superobject* hierarchy;
extern struct rnd* rnd;

typedef void (*object_function)(void* object, void* param);

/* FIX entries */
extern struct array* demo_save_names;
extern struct array* demo_level_names;
extern struct array* level_names;
extern const char* first_level_name;
extern pointer fix_language_offset;
extern unsigned fix_language_count;
extern unsigned fix_texture_count;

/* LVL entries */
extern struct array* family_names;
extern struct array* model_names;
extern struct array* instance_names;
extern struct array* family_colors;

/* Important actors */
extern struct actor* actor_rayman;
extern struct actor* actor_camera;
extern struct actor* actor_global;
extern struct actor* actor_world;
extern struct actor* actor_changemap;

/* */
extern uint8_t previous_engine_mode;
extern unsigned transition_frame;
/* just_entered_mode: true if engine mode differs from previous frame */
#define just_entered_mode(m) ((engine->mode == m) && (previous_engine_mode != m))

/* TAS */
extern struct inputstructure
{
    float joymain_x, joymain_y;
    float joyc_x, joyc_y;
    bool a,b,x,y,z,l,r,S,L,U,D,R;
} input;


void level_read(void);

void level_load(const char* level_name);

#pragma mark - Actor

/**
 * actor_name:
 *  Return the family, model, or instance name of specfied actor.
 */
const char* actor_name(int name, const struct actor* actor);

int actor_dsgvar(const struct superobject* actor_so, unsigned var, int* type, void** data);

#pragma mark - RNG

/**
 * rnd_table_index:
 *  Index the RNG table.
 */
int32_t rnd_table_index(const struct rnd *rnd, unsigned index, unsigned offset);

/**
 * rnd_call:
 *  Call the random number device `n_calls` times.
 */
int32_t rnd_call(const struct rnd *rnd, unsigned n_calls, unsigned index, unsigned mi, unsigned ma);

/* Structure functions */
#include "find.h"

#endif /* game_h */
