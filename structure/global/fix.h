//
//  fix.h
//  r3lib
//
//  Created by Jba03 on 2022-11-21.
//

#ifndef fix_h
#define fix_h

#include "common.h"

#define FIX_STRUCTURE

struct FIX {
    pointer identity_matrix;
    pointer localization_structure;
    
    uint32_t level_name_count;
    uint32_t entry_count;
    uint32_t language_count;
    uint32_t language_offset;
    uint32_t texture_count;
    
    address offset;
};

FIX_STRUCTURE struct FIX* fix_read(const address addr);

#endif /* fix_h */
