#ifndef stObjectType_h
#define stObjectType_h

#include "stLinkedList.h"

#define object_family_name      0
#define object_model_name       1
#define object_instance_name    2

struct stObjectTypeElement {
    readonly pointer next;
    readonly pointer prev;
    readonly pointer father;
    readonly pointer name;
    readonly uint8 priority;
    readonly uint8 identifier;
    padding(2)
} typedef tdstObjectTypeElement;

struct stObjectType {
    readonly tdstDoublyLinkedList family;
    readonly tdstDoublyLinkedList model;
    readonly tdstDoublyLinkedList instance;
} typedef tdstObjectType;

#if USE_FUNCTIONS

/** object_name_type: get the name of an object type with specified id */
const char* object_type_name(int name_type, int id);

#endif

#endif /* stObjectType_h */
