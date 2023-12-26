#ifndef stAIModel_h
#define stAIModel_h

#include "library.hh"

typedef struct stAIModel stAIModel;

struct stAIModel
{
    readonly pointer<> intelligenceBehaviorList;
    readonly pointer<> reflexBehaviorList;
    readonly pointer<> dsgVar;
    readonly pointer<> macroList;
    readonly uint8 secondPass;
    readonly padding(3)
};

#endif /* stAIModel_h */
