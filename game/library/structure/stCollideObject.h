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
#define fnCollideObjectGetElementIndexedSpheres              7
#define collide_object_aabb                 8
#define collide_object_cones                9
#define collide_object_deformationsetinfo   13

typedef struct stCollideObject tdstCollideObject;

struct stCollideObject
{
    readonly int16 numVertices;
    readonly int16 numElements;
    readonly int16 numBoundingBoxes;
    readonly padding(2)
    readonly pointer vertices;
    readonly pointer elementTypes;
    readonly doublepointer elements;
    readonly pointer octree;
    readonly pointer boundingBoxes;
    readonly float32 boundingSphereRadius;
    readonly tdstVector4D boundingSpherePosition;
};

#ifdef USE_FUNCTIONS

#include "stCollideElementIndexedTriangles.h"
#include "stMatrix4D.h"
#include "stVector3D.h"
#include "ray.h"

/** fnCollideObjectGetElementIndexedTriangles: get collide mesh at specified index */
const tdstCollideElementIndexedTriangles* fnCollideObjectGetElementIndexedTriangles(const tdstCollideObject* object, int idx);

/** fnCollideObjectGetElementIndexedSphere: get collide sphere at specified index */
const tdstCollideElementIndexedTriangles* fnCollideObjectGetElementIndexedSphere(const tdstCollideObject* object, int idx);

/** fnCollideObjectIntersectSegment: intersect a collide object with a line segment */
const bool fnCollideObjectIntersectSegment(const tdstCollideObject* object,
                                            const tdstMatrix4D T,
                                            const tdstVector3D sA,
                                            const tdstVector3D sB,
                                            struct ray *out_ray,
                                            tdstVector3D *I);

#endif

#endif /* stCollideObject_h */
