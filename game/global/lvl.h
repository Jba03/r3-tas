//
//  lvl.h
//  r3lib
//
//  Created by Jba03 on 2022-11-21.
//

#ifndef lvl_h
#define lvl_h

#include "common.h"

#define LVL_STRUCTURE

struct LVL {
    char timestamp[0x18];
    
    pointer actual_world;
    pointer dynamic_world;
    pointer inactive_dynamic_world;
    pointer father_sector;
    
    uint32_t submap_position;
    uint32_t texture_count;
    uint32_t num_always;
    
    const char* family_names[2048];
    const char* model_names[2048];
    const char* instance_names[2048];
    
    address offset;
};

LVL_STRUCTURE struct LVL* lvl_read(const uint32_t address);

#endif /* lvl_h */
