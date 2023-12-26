#ifndef stCollideObject_h
#define stCollideObject_h

#include "stVector4D.hh"
#include "library.hh"

#define collide_object_indexed_triangles    1
#define collide_object_facemap              2
#define collide_object_sprite               3
#define collide_object_tmesh                4
#define collide_object_points               5
#define collide_object_lines                6
#define collide_object_indexed_spheres      7
#define collide_object_aabb                 8
#define collide_object_cones                9
#define collide_object_deformationsetinfo   13

typedef struct stCollideObject stCollideObject;

struct stCollideObject
{
    readonly int16 numVertices;
    readonly int16 numElements;
    readonly int16 numBoundingBoxes;
    readonly padding(2)
    readonly pointer<> vertices;
    readonly pointer<> elementTypes;
    readonly doublepointer<> elements;
    readonly pointer<> octree;
    readonly pointer<> boundingBoxes;
    readonly float32 boundingSphereRadius;
    readonly stVector4D boundingSpherePosition;
};

#ifdef USE_FUNCTIONS

#include "stCollideElementIndexedTriangles.hh"
#include "stCollideElementIndexedSpheres.hh"
#include "stMatrix4D.hh"
#include "stVector3D.hh"
#include "ray.hh"

/** fnCollideObjectGetElementIndexedTriangles: get collide mesh at specified index */
const stCollideElementIndexedTriangles* fnCollideObjectGetElementIndexedTriangles(const stCollideObject* object, int idx);

/** fnCollideObjectGetElementIndexedSphere: get collide sphere at specified index */
const stCollideElementIndexedSphere* fnCollideObjectGetElementIndexedSphere(const stCollideObject* object, int idx);

/** fnCollideObjectIntersectSegment: intersect a collide object with a line segment */
const bool fnCollideObjectIntersectSegment(const stCollideObject* object,
                                            const stMatrix4D T,
                                            const stVector3D sA,
                                            const stVector3D sB,
                                            struct ray *out_ray,
                                            stVector3D *I);

#endif

#endif /* stCollideObject_h */
