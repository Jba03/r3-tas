//
//  game.c
//  r3-tas
//
//  Created by Jba03 on 2022-12-19.
//

#include "game.h"
#include "stdgame.h"
#include "log.h"
#include "rnd.h"
#include "engine.h"
#include "brain.h"
#include "mind.h"
#include "intelligence.h"
#include "dsg.h"
//#include "vector3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma mark - Game

struct memory memory;

/* Global structures */
struct engine* engine = NULL;
struct superobject* hierarchy = NULL;
struct superobject* hierarchy2 = NULL;
struct rnd* rnd = NULL;

/* FIX entries */
struct array* demo_save_names = NULL;
struct array* demo_level_names = NULL;
struct array* level_names = NULL;
const char* first_level_name = NULL;
pointer fix_language_offset = 0;
unsigned fix_language_count = 0;
unsigned fix_texture_count = 0;

/* LVL entries */
struct array* family_names = NULL;
struct array* model_names = NULL;
struct array* instance_names = NULL;
struct array* family_colors = NULL;

/* Important actors */
struct actor* actor_rayman = NULL;
struct actor* actor_camera = NULL;
struct actor* actor_global = NULL;
struct actor* actor_world = NULL;
struct actor* actor_changemap = NULL;

/* */
uint8_t previous_engine_mode = 0;
unsigned transition_frame = 0;

/* TAS */
struct inputstructure input;

static bool print = false;

static uint32_t color_table[] =
{
    host_byteorder_32(0x20ff20c0),
    host_byteorder_32(0x5050ffc0),
    host_byteorder_32(0xffa020c0),
    host_byteorder_32(0xff20ffc0),
    host_byteorder_32(0x10af10c0),
    host_byteorder_32(0xfff020c0),
    host_byteorder_32(0xa000ffc0),
    host_byteorder_32(0xb02040c0),
    host_byteorder_32(0x20ffd0c0),
};

static void read_fix(const uint8_t* fix)
{
    info(BOLD COLOR_GREEN "FIX @ [0x%X : %p]\n", offset(fix), fix);
    
    fix += (4 * 4) + (4 * 4);
    
    pointer identity_matrix = host_byteorder_32(*(uint32_t*)fix) & 0xFFFFFFF; fix += 4;
    pointer localization_structure = host_byteorder_32(*(uint32_t*)fix) & 0xFFFFFFF; fix += 4;
    uint32_t level_name_count = host_byteorder_32(*(uint32_t*)fix); fix += 4;
    uint32_t entry_count = host_byteorder_32(*(uint32_t*)fix); fix += 4;
    
    demo_save_names = array_create();
    demo_level_names = array_create();
    level_names = array_create();
    
    /* Demo save names */
    if (print) info(COLOR_GREEN "\t[Demo saves]\n");
    for (int i = 0; i < entry_count; i++, fix += 0xC)
    {
        const char* entry = strdup((const char*)fix);
        if (print) info(COLOR_PINK "\t[fix @ %X]: " COLOR_BLUE "%s\n", offset(fix), entry);
        array_push(demo_save_names, (void*)entry);
    }
    
    /* Demo level names */
    if (print) info(COLOR_GREEN "\t[Demo level names]\n");
    for (int i = 0; i < entry_count; i++, fix += 0xC)
    {
        const char* entry = strdup((const char*)fix);
        if (print) info(COLOR_PINK "\t[fix @ %X]: " COLOR_BLUE "%s\n", offset(fix), entry);
        array_push(demo_level_names, (void*)entry);
    }
    
    /* All level names */
    if (print) info(COLOR_GREEN "\t[Level names]\n");
    for (int i = 0; i < level_name_count; i++, fix += 0x1E)
    {
        const char* entry = strdup((const char*)fix);
        if (print) info(COLOR_PINK "\t[fix @ %X]: " COLOR_BLUE "%s\n", offset(fix), entry);
        array_push(level_names, (void*)entry);
    }
    
    first_level_name = strdup((const char*)fix);
    fix += 0x1E + 2;
    
    fix_language_count = host_byteorder_32(*(uint32_t*)fix); fix += 4;
    fix_language_offset = host_byteorder_32(*(uint32_t*)fix) & 0xFFFFFFF; fix += 4;
    fix_texture_count = host_byteorder_32(*(uint32_t*)fix); fix += 4;
}

static void read_lvl(const uint8_t* lvl)
{
    info(BOLD COLOR_GREEN "LVL @ [0x%X : %p]\n", offset(lvl), lvl);
    
    lvl += 4 * 4;
    lvl += 0x18; /* timestamp */
    lvl += 0xF0;
    
    uint32_t texture_count = host_byteorder_32(*(uint32_t*)lvl); lvl += 4;
    uint32_t total_texture_count = texture_count - fix_texture_count;
    lvl += total_texture_count * 4;
    lvl += total_texture_count * 4;
    
    lvl += 4 * 6; /* Global pointers? */
    lvl += 4 * 3; /* Spawnable actors */
    lvl += 4 * 3; /* Spawnable actors */
    
    family_names = array_create();
    model_names = array_create();
    instance_names = array_create();
    family_colors = array_create();
    
    /* Read object types */
    for (unsigned int i = 0; i < 3; i++)
    {
        const uint8_t* off = lvl;
        const pointer first = host_byteorder_32(*(uint32_t*)lvl) & 0xFFFFFFF; lvl += 4;
        const pointer last = host_byteorder_32(*(uint32_t*)lvl) & 0xFFFFFFF; lvl += 4;
        const uint32_t n_names = host_byteorder_32(*(uint32_t*)lvl); lvl += 4;
        
        if (print) info(COLOR_GREEN "\t[%s names (%d)]\n", (const char* []){"Family", "Model", "Instance"}[i], n_names);
        
        lvl = memory.base + first;
        for (unsigned n = 0; n < n_names; n++)
        {
            const pointer next = host_byteorder_32(*(uint32_t*)lvl) & 0xFFFFFFF; lvl += 4;
            const pointer prev = host_byteorder_32(*(uint32_t*)lvl) & 0xFFFFFFF; lvl += 4;
            const pointer header_offset = host_byteorder_32(*(uint32_t*)lvl) & 0xFFFFFFF; lvl += 4;
            const pointer name_offset = host_byteorder_32(*(uint32_t*)lvl) & 0xFFFFFFF; lvl += 4;
            
            void* name = strdup((const char*)(memory.base + name_offset));
            if (i == 0) array_push(family_names, name);
            if (i == 1) array_push(model_names, name);
            if (i == 2) array_push(instance_names, name);
            
            if (i == 0)
            {
                int select = (array_element_count(family_names) + n) % 9;
                uint32_t* color = &color_table[select];
                array_push(family_colors, color);
            }
            
            if (print) info(COLOR_PINK "\t%s\n", name);
            
            if (next == 0x00) break;
            lvl = memory.base + next;
        }
        
        lvl = off + 3 * 4;
    }
}

void level_read()
{
    const uint8_t* fix = memory.base + (host_byteorder_32(*(uint32_t*)(memory.base + GCN_POINTER_FIX)) & 0xFFFFFFF);
    const uint8_t* lvl = memory.base + (host_byteorder_32(*(uint32_t*)(memory.base + GCN_POINTER_LVL)) & 0xFFFFFFF);
    
    read_fix(fix);
    read_lvl(lvl);
}

void level_load(const char* level_name)
{
    memset((char*)engine->next_level_name, 0, 30);
    strcpy((char*)engine->next_level_name, level_name);
    engine->mode = 6; /* Change map */
}

const char* actor_name(int name, const struct actor* actor)
{
    const struct standard_game_info* stdgame = pointer(actor->stdgame);
    if (!stdgame) return "NULL";
    
    int32_t family = host_byteorder_32(stdgame->family_type);
    int32_t model = host_byteorder_32(stdgame->model_type);
    int32_t instance = host_byteorder_32(stdgame->instance_type);
    
    switch (name)
    {
        case actor_family_name: return family >= 0 && family < array_element_count(family_names) ? array_get(family_names, family) : NULL;
        case actor_model_name: return model >= 0 && model < array_element_count(model_names) ? array_get(model_names, model) : NULL;
        case actor_instance_name: return instance >= 0 && instance < array_element_count(instance_names) ? array_get(instance_names, instance) : NULL;
    }
    
    return NULL;
}

int actor_dsgvar(const struct superobject* actor_so, unsigned var, int* type, void** data)
{
    const struct actor* actor = superobject_data(actor_so);
    if (!actor) return -1;

    const struct brain* brain = actor_brain(actor);
    if (!brain) return -1;
    
    const struct mind* mind = pointer(brain->mind);
    if (!mind) return -1;
    
    const struct dsgmem* dsgmem = pointer(mind->dsgmemory);
    if (!dsgmem) return -1;
    
    const struct dsgvar* dsgvars = doublepointer(dsgmem->dsgvars);
    if (!dsgvars) return -1;
    
    if (var >= dsgvars->info_length) return -1;
    
    const struct dsgvar_info* variable = (struct dsgvar_info*)pointer(dsgvars->info) + var;
    *type = host_byteorder_32(variable->type);
    
    const uint8_t* dataptr = (uint8_t*)pointer(dsgmem->buffer_current);
    if (!dataptr) return -1;
    
    *data = (void*)(data + host_byteorder_32(variable->mem_offset));
    return 0;
}


/**
 * RNG calls per actor type
 *
 *  - Rayman calls RNG whenever he shoots his fist, or whenever his idle animation changes.
 *  - Collecting a yellow or red gem calls RNG five times.
 *  - Collecting a crown calls RNG four times.
 *  - Gems that disappear after a set duration call RNG once per frame when they flicker.
 *  - Matuvus (if visible) call RNG once on animation change.
 *  - Cages call RNG once when broken.
 *  - Certain lamps (ie. the ones in the beginning of TLS1) call RNG when the light changes.
 *
 *  - Razoff #1 calls RNG when he shoots
 *  - Razoff #2's wrecking ball calls RNG twice when it hits Rayman
 *  - Razoff #2 calls RNG once when he falls off the wrecking ball
 *
 *  - The cannonball in Sea_10 calls the RNG twice when frame % 4 == 0.
 *
 *  - Riding the snowboard normally, updates RNG a total of three times per frame:
 *    twice for the snow particles, once for the animation of rayman bobbing up and down.
 *    When in the air, twice. After hitting a fence, three times.
 *
 * LaunchAGO seems to be the function responsible for the internal RNG update in most cases.
 */

int32_t rnd_table_index(const struct rnd *rnd, unsigned index, int offset)
{
    if (host_byteorder_32(rnd->table) == 0x00) return 0;
    
    /* The RNG table index is fetched from a table of 50 RNG "channels", */
    /* index 0 being the most common (if not the only) used by the game. */
    int32_t idx = host_byteorder_32(*(int32_t*)(rnd->table_indices + index));
    
    printf("current index: %d, %d, off: %d\n", idx, max(0, idx + offset), offset);
    /* The RNG table of 10000 entries is then indexed by previous index, together with an optional offset. */
    int32_t value = host_byteorder_32(*(int32_t*)((uint8_t*)pointer(rnd->table) + max(0, idx + offset) * 4));
    
    return value;
}

int32_t rnd_call(const struct rnd *rnd, unsigned n_calls, unsigned index, unsigned mi, unsigned ma)
{
    int32_t value = 0;
    int32_t tmax = host_byteorder_32(rnd->table_max);
    
    for (unsigned n = 0; n < n_calls; n++)
        value = (mi + ((ma + 1 - mi) * rnd_table_index(rnd, index, n)) / (tmax + 1));
    
    return value;
}
