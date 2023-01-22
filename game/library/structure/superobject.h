#ifndef superobject_h
#define superobject_h

#include "structure.h"

#define superobject_type_none                       0x0
#define superobject_type_world                      0x1
#define superobject_type_actor                      0x2
#define superobject_type_sector                     0x4
#define superobject_type_physical_object            0x8
#define superobject_type_ipo                        0x20
#define superobject_type_ipo2                       0x40
#define superobject_type_geometric_object           0x400
#define superobject_type_geometric_shadow_object    0x80000

struct superobject
{
    readonly uint32_t type;
    readonly pointer data;
    readonly pointer first_child;
    readonly pointer last_child;
    readonly int32_t n_children;
    readonly pointer next;
    readonly pointer prev;
    readonly pointer parent;
    readonly pointer transform_local;
    readonly pointer transform_global;
    readonly int32_t prev_frame_processed;
    readonly int32_t draw_flags;
    readonly uint32_t flags;
    readonly pointer visual_bbox;
    readonly pointer collide_bbox;
    readonly struct vector3 semi_lookat;
    readonly float transparency;
    readonly uint32_t outline_color;
    readonly int32_t display_priority;
    readonly int32_t ilstatus; /* ? */
    readonly struct vector3 ambient_color;
    readonly struct vector3 parallel_direction;
    readonly struct vector3 parallel_color;
    readonly uint8_t superimpose;
    readonly uint8_t is_superobject;
    readonly uint8_t transition;
    padding(1)
};

#define superobject_type(so) host_byteorder_32(so->type)
#define superobject_n_children(so) host_byteorder_32(so->n_children)
#define superobject_flags(so) host_byteorder_32(so->flags)
#define superobject_drawflags(so) host_byteorder_32(so->draw_flags)
#define superobject_type(so) host_byteorder_32(so->type)

#define superobject_data(so) ((void*)pointer(so->data))
#define superobject_next(so) ((struct superobject*)pointer(so->next))
#define superobject_prev(so) ((struct superobject*)pointer(so->prev))
#define superobject_parent(so) ((struct superobject*)pointer(so->parent))
#define superobject_first_child(so) ((struct superobject*)pointer(so->first_child))
#define superobject_last_child(so) ((struct superobject*)pointer(so->last_child))

/**
 * superobject_for_each:
 *  Recurse the hierarchy under `root`.
 */
#define superobject_for_each(root, obj) \
    struct superobject* obj = (struct superobject*)pointer(root->first_child); \
    for (; obj != NULL; obj = (struct superobject*)pointer(obj->next))

/**
 * superobject_for_each_type:
 *  Recurse the hierarchy under `root`, finding only objects of specified type.
 */
#define superobject_for_each_type(so_type, root, obj) \
    struct superobject* obj = superobject_first_child(root); \
    for (; obj != NULL; obj = superobject_next(obj)) \
    if (superobject_type(obj) == so_type)

/**
 * superobject_for_each_type_but:
 *  Recurse the hierarchy under `root`, finding only objects that are -not- of specified type.
 */
#define superobject_for_each_type_but(type, root, obj) \
    struct superobject* obj = (struct superobject*)pointer(root->first_child); \
    for (; obj != NULL; obj = (struct superobject*)pointer(obj->next)) \
    if (host_byteorder_32(obj->type) != type)

/**
 * superobject_typename:
 *  Return the name of specified superobject's type
 */
static inline const char* superobject_typename(struct superobject *so)
{
    if (!so) return NULL;
    switch (superobject_type(so))
    {
        case superobject_type_none: return "Dummy";
        case superobject_type_world: return "World";
        case superobject_type_actor: return "Actor";
        case superobject_type_sector: return "Sector";
        case superobject_type_physical_object: return "PhysicalObject";
        case superobject_type_ipo: return "IPO";
        case superobject_type_ipo2: return "IPO.2";
        case superobject_type_geometric_object: return "GeometricObject";
        case superobject_type_geometric_shadow_object: return "GeometricShadowObject";
        default: return "Invalid";
    }
}

#endif /* superobject_h */
