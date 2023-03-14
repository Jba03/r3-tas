#ifndef geometric_object_h
#define geometric_object_h

#include "structure.h"

struct geometric_object
{
    readonly pointer points;
    readonly pointer normals;
    readonly doublepointer transparency;
    readonly pointer element_types;
    readonly doublepointer elements;
    readonly pointer edges;
    readonly pointer aabbs;
    readonly uint32 type;
    readonly int16 n_points;
    readonly int16 n_elements;
    readonly int16 n_edges;
    readonly int16 n_aabb;
    readonly float32 bounding_spheree_radius;
    padding(4)
    struct vector4 bounding_sphere_center;
    readonly pointer edgelist_di;
    readonly int16 n_edges_di;
    readonly int16 n_octree_edges;
    readonly int32 shadow_drawing;
    readonly int32 shadow_construction;
    readonly pointer sdc;
    readonly uint32 is_static;
    readonly uint32 displaylist;
    readonly uint8 sinus;
    padding(3)
};

#endif /* geometric_object_h */
