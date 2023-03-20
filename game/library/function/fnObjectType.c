//
//  object_type.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-21.
//

#include "stObjectType.h"
#include "game.h"

#include <stdio.h>

/** object_name_type: get the name of an object type with specified id */
const char* object_type_name(int name_type, int id)
{
    if (!lvl.section_a) return NULL;
    
    tdstDoublyLinkedList list;
    if (name_type == object_family_name) list = lvl.section_a->object_type.family;
    if (name_type == object_model_name) list = lvl.section_a->object_type.model;
    if (name_type == object_instance_name) list = lvl.section_a->object_type.instance;
    
    if (!pointer(list.first) || !pointer(list.last)) return NULL;
    if ((int32)host_byteorder_32(list.n_entries) < 0) return NULL;
    
    if (id >= 0 && id < (int32)host_byteorder_32(list.n_entries))
    {
        int n = 0;
        tdstObjectTypeElement* t = pointer(list.first);
        while (n++ < id) if(!(t = pointer(t->next))) break;
        if (!t) return NULL;
        return pointer(t->name);
    }
    
    return NULL;
}
