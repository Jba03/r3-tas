//
//  game.c
//  r3-tas
//
//  Created by Jba03 on 2022-12-19.
//

#include "game.h"
#include "stStandardGameInfo.h"
#include "log.h"
#include "stRandom.h"
#include "stEngineStructure.h"
#include "stBrain.h"
#include "stMind.h"
#include "stIntelligence.h"
#include "stDsg.h"
#include "fix.h"
#include "lvl.h"
//#include "vector3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct memory memory;

#pragma mark - Globals

/* Engine structure */
tdstEngineStructure* engine = NULL;
/* Superobject hierarchy */
tdstSuperObject* hierarchy = NULL;
/* Dynamic world: the world in which active game objects reside */
tdstSuperObject* dynamic_world = NULL;
/* Inactive dynamic world: the world in which non-loaded objects reside */
tdstSuperObject* inactive_dynamic_world = NULL;
/* Father sector: sector in which all collision geometry is stored */
tdstSuperObject* father_sector = NULL;
/* Input structure: global input data table */
tdstInputStructure* input_struct = NULL;
/* RND: random number device */
tdstRandom* rnd = NULL;
/* FIX: fixed memory */
struct fix fix;
/* LVL: level memory */
struct lvl lvl;

#pragma mark - Important actors

tdstEngineObject* actor_rayman = NULL;
tdstEngineObject* actor_camera = NULL;
tdstEngineObject* actor_global = NULL;
tdstEngineObject* actor_world = NULL;
tdstEngineObject* actor_changemap = NULL;

#pragma mark - Engine

/* */
uint8_t previous_engine_mode = 0;
char previous_level_name[30];
unsigned transition_frame = 0;

/* TAS */
struct inputstructure input;

static const uint32 color_table[] =
{
    0xc020ff20,
    0xc0ff5050,
    0xc020a0ff,
    0xc0ff20ff,
    0xc010af10,
    0xc020f0ff,
    0xc0ff00a0,
    0xc04020b0,
    0xc0d0ff20,
};

void level_read()
{
    const uint8* fixptr = memory.base + (host_byteorder_32(*(uint32_t*)(memory.base + GCN_POINTER_FIX)) & 0xFFFFFFF);
    const uint8* lvlptr = memory.base + (host_byteorder_32(*(uint32_t*)(memory.base + GCN_POINTER_LVL)) & 0xFFFFFFF);
    
    #pragma mark FIX
    {
        info(BOLD COLOR_GREEN "FIX @ [0x%X : %p]\n", offset(fixptr), fixptr);
        
        fix.header = (struct fix_header*)fixptr;
        
        uint8* offset = (uint8*)(fix.header + 1);
        /* Skip demo save names */
        offset += 12 * host_byteorder_32(fix.header->demo_name_count);
        /* Skip demo level names */
        offset += 12 * host_byteorder_32(fix.header->demo_name_count);
        /* Skip level names (these are derived from the engine struct) */
        offset += 30 * host_byteorder_32(fix.header->level_name_count);
        
        fix.trailer = (struct fix_trailer*)offset;
    }
    
    #pragma mark LVL
    {
        info(BOLD COLOR_GREEN "LVL @ [0x%X : %p]\n", offset(lvlptr), lvlptr);
        
        lvl.header = (struct lvl_header*)lvlptr;
        
        uint8* offset = (uint8*)(lvl.header + 1);
        /* Calculate total texture count: this is the number of textures in the level aside (duplicates?) in fixed memory */
        uint32 n_textures = host_byteorder_32(lvl.header->texture_count) - host_byteorder_32(fix.trailer->texture_count);
        /* Skip textures */
        offset += n_textures * 4 * 2;
        /* Read dynamically aligned section 1 */
        lvl.section_a = (struct lvl_section_a*)offset;
    }
}

uint32 color_table_index(unsigned idx)
{
    return color_table[idx % 9];
}

uint32 actor_color(const tdstEngineObject* actor)
{
    if (!actor) return 0x80808080;
    
    const tdstStandardGameInfo* stdgame = pointer(actor->stdgame);
    if (!stdgame) return 0x80808080;
    
    unsigned n = host_byteorder_32(stdgame->family_type);
    return color_table_index(2 * n + 1);
}

void game_memory_dump()
{
    char path[4096];
    sprintf(path, "%s/memory.bin", LIBR3TAS_DIR);
    FILE* fp = fopen(path, "wb");
    fwrite(memory.base, sizeof(uint8), 24 * 1000 * 1000, fp);
    fclose(fp);
}
