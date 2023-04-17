#ifndef stObjectType_h
#define stObjectType_h

#include "stLinkedList.h"

#define object_family_name      0
#define object_model_name       1
#define object_instance_name    2

typedef struct stObjectTypeElement tdstObjectTypeElement;
typedef struct stObjectType tdstObjectType;

struct stObjectTypeElement
{
    readonly pointer next;
    readonly pointer prev;
    readonly pointer father;
    readonly pointer name;
    readonly uint8 priority;
    readonly uint8 identifier;
    readonly padding(2)
};

struct stObjectType
{
    readonly tdstDoublyLinkedList family;
    readonly tdstDoublyLinkedList model;
    readonly tdstDoublyLinkedList instance;
};

#ifdef USE_FUNCTIONS

/** object_name_type: get the name of an object type with specified id */
const char* fnObjectTypeGetName(const tdstObjectType* objectType, int name_type, int id);

#endif

#endif /* stObjectType_h */
