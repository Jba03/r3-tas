#ifndef stAIModel_h
#define stAIModel_h

#include "structure.h"

struct stAIModel {
    readonly pointer intelligenceBehaviorList;
    readonly pointer reflexBehaviorList;
    readonly pointer dsgVar;
    readonly pointer macroList;
    readonly uint8 secondPass;
    padding(3)
} typedef tdstAIModel;

#endif /* stAIModel_h */
