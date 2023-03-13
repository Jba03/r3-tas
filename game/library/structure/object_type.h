#ifndef object_type_h
#define object_type_h

#include "list.h"

#define object_family_name      0
#define object_model_name       1
#define object_instance_name    2

struct object_type
{
    readonly pointer next;
    readonly pointer prev;
    readonly pointer father;
    readonly pointer name;
    readonly uint8 priority;
    readonly uint8 identifier;
    padding(2)
};

struct object_type_structure
{
    readonly doubly_linked_list family;
    readonly doubly_linked_list model;
    readonly doubly_linked_list instance;
};

#if USE_FUNCTIONS

/** object_name_type: get the name of an object type with specified id */
const char* object_type_name(int name_type, int id);

#endif

#endif /* object_type_h */
