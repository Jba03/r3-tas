//
//  behavior.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-27.
//

#ifndef behavior_h
#define behavior_h

#include "global.h"
#include "script.h"

#define BEHAVIOR

struct Behavior {
    pointer ptr_scripts;
    pointer ptr_script_first;
    
    struct Script** scripts;
    uint8_t n_scripts;
    const char* name;
    
    address offset;
};

BEHAVIOR struct Behavior* behavior_read(const address addr);

#endif /* behavior_h */
