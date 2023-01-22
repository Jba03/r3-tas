#ifndef collide_object_h
#define collide_object_h

#include "vector4.h"
#include "structure.h"

#define collide_object_indexed_triangles    1
#define collide_object_facemap              2
#define collide_object_sprite               3
#define collide_object_tmesh                4
#define collide_object_points               5
#define collide_object_lines                6
#define collide_object_spheres              7
#define collide_object_aabb                 8
#define collide_object_cones                9
#define collide_object_deformationsetinfo   13

struct collide_object
{
    readonly int16_t n_vertices;
    readonly int16_t n_elements;
    readonly int16_t n_bboxes;
    padding(2)
    readonly pointer vertices;
    readonly pointer element_types;
    readonly doublepointer elements;
    readonly pointer octree;
    readonly pointer bboxes;
    readonly float bounding_sphere_radius;
    readonly struct vector4 bounding_sphere_position;
};

#endif /* collide_object_h */