#ifndef stAIModel_h
#define stAIModel_h

#include "structure.h"

typedef struct stAIModel tdstAIModel;

struct stAIModel
{
    readonly pointer intelligenceBehaviorList;
    readonly pointer reflexBehaviorList;
    readonly pointer dsgVar;
    readonly pointer macroList;
    readonly uint8 secondPass;
    readonly padding(3)
};

#endif /* stAIModel_h */
