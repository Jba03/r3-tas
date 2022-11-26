//
//  brain.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-26.
//

#ifndef brain_h
#define brain_h

#include "mind.h"

#define BRAIN

struct Brain {
    pointer ptr_mind;
    pointer ptr_collide_material;
    
    struct Mind* mind;
    
    uint8_t warn_mechanics;
    uint8_t active_during_transition;
    
    address offset;
};

BRAIN struct Brain *brain_read(const address addr);

#endif /* brain_h */
