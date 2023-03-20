#ifndef stAlways_h
#define stAlways_h

#include "structure.h"
#include "stLinkedList.h"

struct stAlways {
    readonly uint32 n_always;
    readonly tdstDoublyLinkedList always_models;
    readonly pointer always_superobject; /* ::superobject[n_always] */
    readonly pointer always_actors; /* ::actor[n_always] */
    readonly pointer always_generator_superobjects; /* superobject[n_always] */
} typedef tdstAlways;

#endif /* stAlways_h */
