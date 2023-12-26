//
//  object_type.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-21.
//

#include "stObjectType.hh"
#include "memory.hh"

#include <stdio.h>

/** fnObjectTypeGetName: get the name of an object type with specified id */
const char* fnObjectTypeGetName(const tdstObjectType* objectType, int name_type, int id)
{
    tdstDoublyLinkedList list;
    if (name_type == object_family_name) list = objectType->family;
    if (name_type == object_model_name) list = objectType->model;
    if (name_type == object_instance_name) list = objectType->instance;
    
    if (!pointer(list.first) || !pointer(list.last)) return NULL;
    if ((int32)host_byteorder_32(list.numEntries) < 0) return NULL;
    
    if (id >= 0 && id < (int32)host_byteorder_32(list.numEntries))
    {
        int n = 0;
        tdstObjectTypeElement* t = pointer(list.first);
        while (n++ < id) if(!(t = pointer(t->next))) break;
        if (!t) return NULL;
        return pointer(t->name);
    }
    
    return NULL;
}
