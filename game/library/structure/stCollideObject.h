#ifndef stCollideObject_h
#define stCollideObject_h

#include "stVector4D.h"
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

struct stCollideObject {
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
    readonly tdstVector4D bounding_sphere_position;
} typedef tdstCollideObject;

#if USE_FUNCTIONS

#include "stCollideElementIndexedTriangles.h"
#include "stMatrix4D.h"
#include "stVector3D.h"
#include "ray.h"

/** collide_object_mesh: get collide mesh at specified index */
const tdstCollideElementIndexedTriangles* collide_object_mesh(const tdstCollideObject* object, int idx);

/** collide_object_sphere: get collide sphere at specified index */
const tdstCollideElementIndexedTriangles* collide_object_sphere(const tdstCollideObject* object, int idx);

/** collide_object_intersect_segment: intersect a collide object with a line segment */
const bool collide_object_intersect_segment(const tdstCollideObject* object,
                                            const tdstMatrix4D T,
                                            const tdstVector3D sA,
                                            const tdstVector3D sB,
                                            struct ray *out_ray,
                                            tdstVector3D *I);

#endif

#endif /* stCollideObject_h */
