//
//  find.h
//  r3-tas-memorymap
//
//  Created by Jba03 on 2022-12-23.
//

#ifndef find_h
#define find_h

#include "stSuperObject.h"
#include "stStandardGameInfo.h"
#include "stEngineObject.h"
#include "stInstantiatedPhysicalObject.h"

static inline tdstEngineObject* actor_find(int name_type, const char* name, const tdstSuperObject* root)
{
    if (!root) return NULL;
    tdstEngineObject* actor = (tdstEngineObject*)superobject_data(root);
    if (superobject_type(root) == superobject_type_actor && actor)
    {
        const char* compare = actor_name(name_type, actor, objectType);
        if (compare != NULL)
        {
            if (strcmp(name, compare) == 0) return actor;
        }
    }
    
    actor = NULL;
    tdstSuperObject* search = superobject_first_child(root);
    while (search && !actor)
    {
        actor = actor_find(name_type, name, search);
        search = superobject_next(search);
    }
    
    return actor;
}

static inline tdstInstantiatedPhysicalObject* ipo_find(const char* name, tdstSuperObject* root, tdstSuperObject** so)
{
    if (!root) return NULL;
    tdstInstantiatedPhysicalObject* ipo = (tdstInstantiatedPhysicalObject*)superobject_data(root);
    if ((superobject_type(root) == superobject_type_ipo || superobject_type(root) == superobject_type_ipo_mirror) && ipo)
    {
        if (strcmp(name, ipo->name) == 0)
        {
            *so = root;
            return ipo;
        }
    }
    
    ipo = NULL;
    tdstSuperObject* search = superobject_first_child(root);
    while (search && !ipo)
    {
        ipo = ipo_find(name, search, so);
        search = superobject_next(search);
    }
    
    return ipo;
}

#endif /* find_h */
