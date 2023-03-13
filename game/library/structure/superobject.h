#ifndef superobject_h
#define superobject_h

#include "structure.h"
#include "vector3.h"
#include "matrix4.h"

#define superobject_type_none                       0x0
#define superobject_type_world                      0x1
#define superobject_type_actor                      0x2
#define superobject_type_sector                     0x4
#define superobject_type_physical_object            0x8
#define superobject_type_physical_object_mirror     0x10
#define superobject_type_ipo                        0x20
#define superobject_type_ipo_mirror                 0x40
#define superobject_type_special_effect             0x80
#define superobject_type_no_action                  0x100
#define superobject_type_mirror                     0x200

/* TODO: Add draw flags */

static const char * const superobject_typenames[] =
{
    "World", "Actor", "Sector", "PhysicalObject", "PhysicalObject.Mirror",
    "IPO", "IPO.Mirror", "SpecialEffect", "NoAction", "Unknown", "Mirror",
};

struct superobject
{
    readonly uint32 type;
    readonly pointer data;
    readonly pointer first_child;
    readonly pointer last_child;
    readonly int32 n_children;
    readonly pointer next;
    readonly pointer prev;
    readonly pointer parent;
    readonly pointer transform_local;
    readonly pointer transform_global;
    readonly int32 prev_frame_processed;
    readonly int32 draw_flags;
    readonly uint32 flags;
    readonly pointer visual_bbox;
    readonly pointer collide_bbox;
    readonly struct vector3 semi_lookat;
    readonly float32 transparency;
    readonly uint32 outline_color;
    readonly int32 display_priority;
    readonly int32 ilstatus; /* ? */
    readonly struct vector3 ambient_color;
    readonly struct vector3 parallel_direction;
    readonly struct vector3 parallel_color;
    readonly uint8 superimpose;
    readonly uint8 is_superobject;
    readonly uint8 transition;
    padding(1)
};

#if USE_FUNCTIONS

/** superobject_typename: get the typename of a superobject*/
const char* superobject_typename(const struct superobject* so);

/** superobject_name: get the (instance) name of a superobject*/
const char* superobject_name(const struct superobject* so);

/** superobject_matrix_global: get the global world transform matrix of a superobject */
const struct matrix4 superobject_matrix_global(const struct superobject* so);

/** superobject_matrix_local: get the local world transform matrix of a superobject */
const struct matrix4 superobject_matrix_local(const struct superobject* so);

/** sector_by_location: get the sector in which the specified point is located */
const struct superobject* sector_by_location(const struct superobject* father_sector, const struct vector3 point);

#endif

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

#endif /* superobject_h */
