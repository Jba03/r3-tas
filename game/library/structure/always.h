#ifndef always_h
#define always_h

#include "structure.h"
#include "list.h"

struct always
{
    readonly uint32 n_always;
    readonly doubly_linked_list always_models;
    readonly pointer always_superobject; /* ::superobject[n_always] */
    readonly pointer always_actors; /* ::actor[n_always] */
    readonly pointer always_generator_superobjects; /* superobject[n_always] */
};

#endif /* always_h */
