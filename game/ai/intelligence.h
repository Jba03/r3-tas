//
//  intelligence.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-27.
//

#ifndef intelligence_h
#define intelligence_h

#include "memory.h"
#include "behavior.h"

#define INTELLIGENCE

struct Intelligence {
    pointer ptr_model;
    pointer ptr_action_tree;
    pointer ptr_behavior;
    pointer ptr_behavior_last;
    pointer ptr_action_table;
    
    struct AIModel* model;
    struct Behavior* behavior_current;
    struct Behavior* behavior_last;
    
    address offset;
};

INTELLIGENCE struct Intelligence* intelligence_read(const address addr);

#endif /* intelligence_h */
