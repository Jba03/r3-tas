#ifndef stAIModel_h
#define stAIModel_h

#include "structure.h"

struct stAIModel {
    readonly pointer intelligence_behavior_list;
    readonly pointer reflex_behavior_list;
    readonly pointer dsgvar;
    readonly pointer macrolist;
    readonly uint8 secondpass;
    padding(3)
} typedef tdstAIModel;

#endif /* stAIModel_h */
