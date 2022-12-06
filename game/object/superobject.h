//
//  superobject.h
//  r3tools
//
//  Created by Jba03 on 2022-11-12.
//

#ifndef superobject_h
#define superobject_h

#include "global.h"
#include "matrix4.h"

#define SUPEROBJECT

enum SuperObjectType {
    SUPEROBJECT_TYPE_DUMMY                      = 0x0,
    SUPEROBJECT_TYPE_WORLD                      = 0x1,
    SUPEROBJECT_TYPE_ACTOR                      = 0x2,
    SUPEROBJECT_TYPE_SECTOR                     = 0x4,
    SUPEROBJECT_TYPE_PHYSICAL_OBJECT            = 0x8,
    SUPEROBJECT_TYPE_IPO                        = 0x20,
    SUPEROBJECT_TYPE_IPO2                       = 0x40,
    SUPEROBJECT_TYPE_GEOMETRIC_OBJECT           = 0x400,
    SUPEROBJECT_TYPE_GEOMETRIC_SHADOW_OBJECT    = 0x80000,
};

struct SuperObject {
    enum SuperObjectType type;
    pointer data_ptr;
    void* data;
    
    uint32_t n_children;
    pointer child_first;
    pointer child_last;
    pointer sibling_next;
    pointer sibling_prev;
    pointer parent;
    
    struct SuperObject *children[1000];
    
    struct Matrix4 matrix_default;
    struct Matrix4 matrix_static;
    struct Matrix4 matrix_global;
    
    address offset;
};

SUPEROBJECT struct SuperObject* superobject_read(const address addr);

SUPEROBJECT const char* superobject_typename(const struct SuperObject *obj);

SUPEROBJECT void superobject_free(struct SuperObject** obj);

/* superobject_for_each: run a function for every superobject of specified type */
SUPEROBJECT void superobject_for_each(const enum SuperObjectType type,
                                       struct SuperObject* root,
                                       void (*function)(struct SuperObject*, void* param),
                                       void* param);

#endif /* superobject_h */
