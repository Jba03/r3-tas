//
//  superobject_links.h
//  r3tools
//
//  Created by Jba03 on 2022-11-14.
//

#ifndef superobject_links_h
#define superobject_links_h

#include "superobject.h"
#include "transform.h"

#define SUPEROBJECT_LINKS

enum SuperObjectLinkType {
    SUPEROBJECT_LINK_ERROR = 0,
    SUPEROBJECT_LINK_SPO = 1,
    SUPEROBJECT_LINK_CHARACTER = 2,
    SUPEROBJECT_LINK_WAYPOINT = 3,
};

enum SuperObjectLinkPersistence {
    SUPEROBJECT_LINK_PERSISTENCE_UNKNOWN = 0,
    SUPEROBJECT_LINK_PERSISTENCE_PERSISTENT = 1,
    SUPEROBJECT_LINK_PERSISTENCE_NONE = 2,
};

struct SuperObjectLinks {
    struct SuperObject* superobject;
    struct Transform* transform_initial;
    uint32_t n_linked_objects;
    uint32_t n_links_allocated;
    void** linked_object;
    enum SuperObjectLinkType obj_type;
    struct Transform* linked_obj_transform_initial;
    enum SuperObjectLinkPersistence persistence;
};

SUPEROBJECT_LINKS struct SuperObjectLinks *superobject_links_read(struct Stream *stream);

#endif /* superobject_links_h */
