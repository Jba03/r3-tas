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
    readonly int16 n_vertices;
    readonly int16 n_elements;
    readonly int16 n_bboxes;
    padding(2)
    readonly pointer vertices;
    readonly pointer element_types;
    readonly doublepointer elements;
    readonly pointer octree;
    readonly pointer bboxes;
    readonly float32 bounding_sphere_radius;
    readonly struct vector4 bounding_sphere_position;
};

#if USE_FUNCTIONS

#include "collide_mesh.h"

/** collide_object_mesh: get collide mesh at specified index */
const struct collide_mesh* collide_object_mesh(const struct collide_object* object, int idx);

/** collide_object_sphere: get collide sphere at specified index */
const struct collide_mesh* collide_object_sphere(const struct collide_object* object, int idx);

#endif

#endif /* collide_object_h */
