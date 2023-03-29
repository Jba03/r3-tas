#ifndef stAlways_h
#define stAlways_h

#include "structure.h"
#include "stLinkedList.h"

struct stAlways {
    readonly uint32 numAlways;
    readonly tdstDoublyLinkedList alwaysModels;
    readonly pointer alwaysSuperobject; /* ::superobject[n_always] */
    readonly pointer alwaysActors; /* ::actor[n_always] */
    readonly pointer alwaysGeneratorSuperobjects; /* superobject[n_always] */
} typedef tdstAlways;

#endif /* stAlways_h */
