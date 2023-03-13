//
//  find.h
//  r3-tas-memorymap
//
//  Created by Jba03 on 2022-12-23.
//

#ifndef find_h
#define find_h

#include "superobject.h"
#include "stdgame.h"
#include "actor.h"
#include "ipo.h"

static inline struct actor* actor_find(int name_type, const char* name, const struct superobject* root)
{
    if (!root) return NULL;
    struct actor* actor = (struct actor*)superobject_data(root);
    if (superobject_type(root) == superobject_type_actor && actor)
    {
        const char* compare = actor_name(name_type, actor);
        if (compare != NULL)
        {
            if (strcmp(name, compare) == 0) return actor;
        }
    }
    
    actor = NULL;
    struct superobject* search = superobject_first_child(root);
    while (search && !actor)
    {
        actor = actor_find(name_type, name, search);
        search = superobject_next(search);
    }
    
    return actor;
}

static inline struct ipo* ipo_find(const char* name, struct superobject* root, struct superobject** so)
{
    if (!root) return NULL;
    struct ipo* ipo = (struct ipo*)superobject_data(root);
    if ((superobject_type(root) == superobject_type_ipo || superobject_type(root) == superobject_type_ipo_mirror) && ipo)
    {
        if (strcmp(name, ipo->name) == 0)
        {
            *so = root;
            return ipo;
        }
    }
    
    ipo = NULL;
    struct superobject* search = superobject_first_child(root);
    while (search && !ipo)
    {
        ipo = ipo_find(name, search, so);
        search = superobject_next(search);
    }
    
    return ipo;
}

#endif /* find_h */
