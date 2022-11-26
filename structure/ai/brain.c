//
//  brain.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-26.
//

#include "brain.h"
#include "stream.h"

BRAIN struct Brain *brain_read(const address addr)
{
    struct Brain *brain = malloc(sizeof *brain);
    brain->offset = addr;
    
    struct Stream* stream = stream_open(addr);
    brain->ptr_mind = readpointer();
    brain->ptr_collide_material = readpointer();
    brain->warn_mechanics = read8();
    brain->active_during_transition = read8();
    stream_close(stream);
    
    if (brain->ptr_mind != 0x00)
        brain->mind = mind_read(brain->ptr_mind);
    
    return brain;
}
