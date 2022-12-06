//
//  behavior.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-27.
//

#include "behavior.h"

BEHAVIOR struct Behavior* behavior_read(const address addr)
{
    struct Behavior* behavior = malloc(sizeof *behavior);
    behavior->offset = addr;
    
    struct Stream* stream = stream_open(addr);
    behavior->name = readstring(0x100);
    behavior->ptr_scripts = readpointer();
    behavior->ptr_script_first = readpointer();
    behavior->n_scripts = read8();
    
    behavior->scripts = malloc(sizeof(struct Script*) * behavior->n_scripts);
    if (behavior->ptr_scripts != 0x00 && behavior->n_scripts > 0)
    {
        for (unsigned i = 0; i < behavior->n_scripts; i++)
            behavior->scripts[i] = script_read(behavior->ptr_scripts + 4 * i);
    }
    //behavior->scripts[0] = script_read(behavior->ptr_script_first);
    
    stream_close(stream);
    
    return behavior;
}
