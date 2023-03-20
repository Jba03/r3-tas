#ifndef stIntelligence_h
#define stIntelligence_h

#include "structure.h"

struct stIntelligence {
    readonly doublepointer script_ai;
    readonly pointer current_tree;
    readonly pointer current_behavior;
    readonly pointer previous_behavior;
    readonly pointer action_table;
    readonly uint32 initialize_behavior;
} typedef tdstIntelligence;

#endif /* stIntelligence_h */
