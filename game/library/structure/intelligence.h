#ifndef intelligence_h
#define intelligence_h

#include "structure.h"

struct intelligence
{
    readonly doublepointer script_ai;
    readonly pointer current_tree;
    readonly pointer current_behavior;
    readonly pointer previous_behavior;
    readonly pointer action_table;
    readonly uint32 initialize_behavior;
};

#endif /* intelligence_h */
