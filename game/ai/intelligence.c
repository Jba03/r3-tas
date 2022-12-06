//
//  intelligence.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-27.
//

#include "intelligence.h"

#pragma mark - Intelligence

INTELLIGENCE struct Intelligence* intelligence_read(const address addr)
{
    struct Intelligence* intelligence = malloc(sizeof *intelligence);
    
    struct Stream* stream = stream_open(addr);
    intelligence->ptr_model = readpointer();
    intelligence->ptr_action_tree = readpointer();
    intelligence->ptr_behavior = readpointer();
    intelligence->ptr_behavior_last = readpointer();
    intelligence->ptr_action_table = readpointer();
    stream_close(stream);
    
    if (intelligence->ptr_behavior != 0x00)
        intelligence->behavior_current = behavior_read(intelligence->ptr_behavior);
    
    if (intelligence->ptr_behavior_last != 0x00)
        intelligence->behavior_last = behavior_read(intelligence->ptr_behavior_last);
    
    return intelligence;
}
