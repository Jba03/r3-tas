#ifndef stAlways_h
#define stAlways_h

#include "library.hh"
#include "stLinkedList.hh"
#include "stSuperObject.hh"

struct stAlways {
    readonly uint32 numAlways;
    readonly stDoublyLinkedList alwaysModels;
    readonly pointer<stSuperObject> alwaysSuperobject; /* ::superobject[n_always] */
    readonly pointer<stEngineObject> alwaysActors; /* ::actor[n_always] */
    readonly pointer<stSuperObject> alwaysGeneratorSuperobjects; /* superobject[n_always] */
};

#endif /* stAlways_h */
