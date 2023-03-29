#ifndef stSuperObject_h
#define stSuperObject_h

#include "structure.h"
#include "stVector3D.h"
#include "stMatrix4D.h"

/* TYPES */
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

/* FLAGS */
#define superobject_flag_not_pickable               0x1
#define superobject_flag_hidden                     0x2
#define superobject_flag_no_transformation_matrix   0x4
#define superobject_flag_zoom_instead_of_scale      0x8
#define superobject_flag_type_of_bounding_volume    0x10
#define superobject_flag_superimposed               0x20
#define superobject_flag_no_ray_intersections       0x40
#define superobject_flag_no_shadow                  0x80
#define superobject_flag_semi_lookat                0x100
#define superobject_flag_check_children             0x200
#define superobject_flag_render_on_near_plane       0x400

static const char * const superObjectTypeNames[] =
{
    "World", "Actor", "Sector", "PhysicalObject", "PhysicalObject.Mirror",
    "IPO", "IPO.Mirror", "SpecialEffect", "NoAction", "Unknown", "Mirror",
};

struct stSuperObject {
    readonly uint32 type;
    readonly pointer data;
    readonly pointer firstChild;
    readonly pointer lastChild;
    readonly int32 numChildren;
    readonly pointer next;
    readonly pointer prev;
    readonly pointer parent;
    readonly pointer localTransform;
    readonly pointer globalTransform;
    readonly int32 prevFrameProcessed;
    readonly int32 drawFlags;
    readonly uint32 flags;
    readonly pointer visualBBox;
    readonly pointer collideBBox;
    readonly tdstVector3D semiLookat;
    readonly float32 transparency;
    readonly uint32 outlineColor;
    readonly int32 displayPriority;
    readonly int32 ilstatus; /* ? */
    readonly tdstVector3D ambientColor;
    readonly tdstVector3D parallelDirection;
    readonly tdstVector3D parallelColor;
    readonly uint8 superimpose;
    readonly uint8 isSuperObject;
    readonly uint8 transition;
    padding(1)
} typedef tdstSuperObject;

#if USE_FUNCTIONS

/** fnSuperobjectGetTypename: get the typename of a superobject*/
const char* fnSuperobjectGetTypename(const tdstSuperObject* so);

/** fnSuperobjectGetName: get the (instance) name of a superobject*/
const char* fnSuperobjectGetName(const tdstSuperObject* so);

/** fnSuperobjectGetGlobalMatrix: get the global world transform matrix of a superobject */
tdstMatrix4D fnSuperobjectGetGlobalMatrix(const tdstSuperObject* so);

/** fnSuperobjectGetLocalMatrix: get the local world transform matrix of a superobject */
tdstMatrix4D fnSuperobjectGetLocalMatrix(const tdstSuperObject* so);

/** sector_by_location: get the sector in which the specified point is located */
const tdstSuperObject* sector_by_location(const tdstSuperObject* father_sector, const tdstVector3D point);

#endif

#define superobject_type(so) host_byteorder_32(so->type)
#define superobject_n_children(so) host_byteorder_32(so->numChildren)
#define superobject_flags(so) host_byteorder_32(so->flags)
#define superobject_drawflags(so) host_byteorder_32(so->drawFlags)
#define superobject_type(so) host_byteorder_32(so->type)

#define superobject_data(so) ((void*)pointer(so->data))
#define superobject_next(so) ((tdstSuperObject*)pointer(so->next))
#define superobject_prev(so) ((tdstSuperObject*)pointer(so->prev))
#define superobject_parent(so) ((tdstSuperObject*)pointer(so->parent))
#define superobject_first_child(so) ((tdstSuperObject*)pointer(so->firstChild))
#define superobject_last_child(so) ((tdstSuperObject*)pointer(so->lastChild))

/**
 * superobject_for_each:
 *  Recurse the hierarchy under `root`.
 */
#define superobject_for_each(root, obj) \
    tdstSuperObject* obj = (tdstSuperObject*)pointer(root->firstChild); \
    for (; obj != NULL; obj = (tdstSuperObject*)pointer(obj->next))

/**
 * superobject_for_each_type:
 *  Recurse the hierarchy under `root`, finding only objects of specified type.
 */
#define superobject_for_each_type(so_type, root, obj) \
    tdstSuperObject* obj = superobject_first_child(root); \
    for (; obj != NULL; obj = superobject_next(obj)) \
    if (superobject_type(obj) == so_type)

/**
 * superobject_for_each_type_but:
 *  Recurse the hierarchy under `root`, finding only objects that are -not- of specified type.
 */
#define superobject_for_each_type_but(type, root, obj) \
    tdstSuperObject* obj = (tdstSuperObject*)pointer(root->firstChild); \
    for (; obj != NULL; obj = (tdstSuperObject*)pointer(obj->next)) \
    if (host_byteorder_32(obj->type) != type)

#endif /* stSuperObject_h */
