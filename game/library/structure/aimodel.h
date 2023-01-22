#ifndef aimodel_h
#define aimodel_h

#include "structure.h"

struct aimodel
{
    readonly pointer intelligence_behavior_list;
    readonly pointer reflex_behavior_list;
    readonly pointer dsgvar;
    readonly pointer macrolist;
    readonly uint8_t secondpass;
    padding(3)
};

//struct aiscript
//{
//
//}

#endif /* aimodel_h */
