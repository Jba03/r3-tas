#ifndef stSuperObject_h
#define stSuperObject_h

#include "library.hh"
#include "stVector3D.hh"
#include "stMatrix4D.hh"
#include "stTransform.hh"
#include "stEngineObject.hh"
#include "stSector.hh"
#include "stInstantiatedPhysicalObject.hh"

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

#include <iostream>
#include <map>

struct stSuperObject {
    enum type : uint32_t {
        NONE = 0,
        WORLD = 1,
        ACTOR = 2,
        SECTOR = 4,
        PHYSICAL_OBJECT = 8,
        PHYSICAL_OBJECT_MIRROR = 16,
        IPO = 32,
        IPO_MIRROR = 64,
        SPECIAL_EFFECT = 128,
        NO_ACTION = 256,
        MIRROR = 512,
    };
    
    readonly uint32 type;
    readonly pointer<> data;
    readonly pointer<stSuperObject> firstChild;
    readonly pointer<stSuperObject> lastChild;
    readonly int32 numChildren;
    readonly pointer<stSuperObject> next;
    readonly pointer<stSuperObject> prev;
    readonly pointer<stSuperObject> parent;
    readonly pointer<stTransform> localTransform;
    readonly pointer<stTransform> globalTransform;
    readonly int32 prevFrameProcessed;
    readonly int32 drawFlags;
    readonly uint32 flags;
    readonly pointer<> visualBBox;
    readonly pointer<> collideBBox;
    readonly stVector3D semiLookat;
    readonly float32 transparency;
    readonly uint32 outlineColor;
    readonly int32 displayPriority;
    readonly int32 ilstatus; /* ? */
    readonly stVector3D ambientColor;
    readonly stVector3D parallelDirection;
    readonly stVector3D parallelColor;
    readonly uint8 superimpose;
    readonly uint8 isSuperObject;
    readonly uint8 transition;
    readonly padding(1)
    
    template <typename T> auto linkedObject() -> T* {
        return data;
    }
    
    operator uint64_t() const {
        return (uint64_t)&type;
    }
    
    std::string getTypeName() {
        switch (type) {
            case NONE: return "Dummy Superobject";
            case WORLD: return "World";
            case ACTOR: return "Actor";
            case SECTOR: return "Sector";
            case PHYSICAL_OBJECT: return "PhysicalObject";
            case PHYSICAL_OBJECT_MIRROR: return "PhysicalObject.Mirror";
            case IPO: return "IPO";
            case IPO_MIRROR: return "IPO.Mirror";
            case SPECIAL_EFFECT: return "SpecialEffect";
            case NO_ACTION: return "NoAction";
            case MIRROR: return "Mirror";
            default: return "Unaddressed superobject type " + std::to_string(type);
        }
    }
    
    std::string getName(stObjectType *objectTypes) {
        stEngineObject *actor = data;
        stSector *sector = data;
        stInstantiatedPhysicalObject *ipo = data;
        
        switch (type) {
            case ACTOR: return actor->getName(actor_instance_name, objectTypes);
            case SECTOR: return sector->getName();
            case IPO: return ipo->getName();
            default: return getTypeName();
        }
    }
    
    template <typename F>
    void recurse(const F& f) {
        _recurse(this, f);
    }
    
    template <typename F>
    void forEachChild(const F& f) {
        for (stSuperObject *ii = firstChild; ii; ii = ii->next) {
            f(ii);
        }
    }
    
    stSuperObject *find(std::string instanceName, stObjectType *objectTypes) {
        stSuperObject *found = nullptr;
        recurse([&](stSuperObject *T) {
            //printf("aa: %s\n", T->getName(objectTypes).c_str());
            if (T->getName(objectTypes) == instanceName) found = T;
        });
        return found;
    }
    
private:
    template <typename F>
    void _recurse(stSuperObject *root, const F& f) {
        for (stSuperObject *ii = root->firstChild; ii; ii = ii->next) {
            f(ii);
            _recurse(ii, f);
        }
    }
};

#ifdef USE_FUNCTIONS

/** fnSuperobjectGetTypename: get the typename of a superobject*/
const char* fnSuperobjectGetTypename(const stSuperObject* so);

/** fnSuperobjectGetName: get the (instance) name of a superobject*/
const char* fnSuperobjectGetName(const stSuperObject* so);

/** fnSuperobjectGetData: get the object data of a superobject */
void *fnSuperobjectGetData(const stSuperObject *so);

/** fnSuperobjectGetGlobalMatrix: get the global world transform matrix of a superobject */
stMatrix4D fnSuperobjectGetGlobalMatrix(const stSuperObject* so);

/** fnSuperobjectGetLocalMatrix: get the local world transform matrix of a superobject */
stMatrix4D fnSuperobjectGetLocalMatrix(const stSuperObject* so);

/** sector_by_location: get the sector in which the specified point is located */
const stSuperObject* sector_by_location(const stSuperObject* father_sector, const stVector3D point);

#endif

#define superobject_type(so) host_byteorder_32(so->type)
#define superobject_n_children(so) host_byteorder_32(so->numChildren)
#define superobject_flags(so) host_byteorder_32(so->flags)
#define superobject_drawflags(so) host_byteorder_32(so->drawFlags)
#define superobject_type(so) host_byteorder_32(so->type)

#define superobject_data(so) ((void*)pointer(so->data))
#define superobject_next(so) ((stSuperObject*)pointer(so->next))
#define superobject_prev(so) ((stSuperObject*)pointer(so->prev))
#define superobject_parent(so) ((stSuperObject*)pointer(so->parent))
#define superobject_first_child(so) ((stSuperObject*)pointer(so->firstChild))
#define superobject_last_child(so) ((stSuperObject*)pointer(so->lastChild))

/**
 * superobject_for_each:
 *  Recurse the hierarchy under `root`.
 */
#define superobject_for_each(root, obj) \
    stSuperObject* obj = (stSuperObject*)pointer(root->firstChild); \
    for (; obj != NULL; obj = (stSuperObject*)pointer(obj->next))

/**
 * superobject_for_each_type:
 *  Recurse the hierarchy under `root`, finding only objects of specified type.
 */
#define superobject_for_each_type(so_type, root, obj) \
    stSuperObject* obj = superobject_first_child(root); \
    for (; obj != NULL; obj = superobject_next(obj)) \
    if (superobject_type(obj) == so_type)

/**
 * superobject_for_each_type_but:
 *  Recurse the hierarchy under `root`, finding only objects that are -not- of specified type.
 */
#define superobject_for_each_type_but(type, root, obj) \
    stSuperObject* obj = (stSuperObject*)pointer(root->firstChild); \
    for (; obj != NULL; obj = (stSuperObject*)pointer(obj->next)) \
    if (host_byteorder_32(obj->type) != type)

#endif /* stSuperObject_h */
