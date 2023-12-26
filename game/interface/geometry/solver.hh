#ifndef solver_h
#define solver_h

#include "stVector3D.hh"
#include "stSuperobject.hh"
#include "stCollideObject.hh"
#include "stDynamics.hh"

struct Sphere
{
    tdstVector3D center;
    float radius;
};

struct Triangle
{
    tdstVector3D A;
    tdstVector3D B;
    tdstVector3D C;
    tdstVector3D N;
};

struct GeometrySolver
{
    tdstVector3D points[10000];
    unsigned int numPoints;
    
    struct Triangle triangles[10000];
    unsigned int numTriangles;
    
    tdstVector3D start;
    tdstVector3D end;
};

void solveGeometry(tdstSuperObject *g, struct GeometrySolver *solver);

struct CollisionResult
{
    tdstVector3D spherePos;
    tdstVector3D segmentStart;
    tdstVector3D segmentEnd;
    struct Triangle triangles[1000];
    int n_triangles;
};

#define COLLISION_TYPE_SPHERE   0
#define COLLISION_TYPE_TRIANGLE 1
#define COLLISION_TYPE_L_EDGE   2
#define COLLISION_TYPE_H_EDGE   3

struct Collision
{
    tdstVector3D normal;
    tdstVector3D contact;
    tdstVector3D translation;
    tdstVector3D movement;
    tdstVector3D endpos;
    unsigned int dynamicType; /* sphere */
    unsigned int staticType; /* triangle, edge */
    float rate;
    
    struct Triangle triangle;
};

struct CollisionRecord
{
#define MAX_COLLISIONS  64
    struct Collision collisions[MAX_COLLISIONS];
    unsigned int numCollisions;
    
};

bool
CollideSphereWithGeometry(const float dynamicSphereRadius,
                          const tdstMatrix4D dynamicSphereStart,
                          const tdstMatrix4D dynamicSphereEnd,
                          const tdstCollideObject *staticObject,
                          const tdstMatrix4D staticObjectMatrix,
                          struct CollisionRecord *record);

void TransformAndCollide(tdstDynamics *d, float dt);

#endif /* solver_h */
