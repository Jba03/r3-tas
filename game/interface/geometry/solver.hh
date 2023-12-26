#ifndef solver_h
#define solver_h

#include "stVector3D.hh"
#include "stSuperObject.hh"
#include "stCollideObject.hh"
#include "stDynamics.hh"

struct Sphere
{
    stVector3D center;
    float radius;
};

struct Triangle
{
    stVector3D A;
    stVector3D B;
    stVector3D C;
    stVector3D N;
};

struct GeometrySolver
{
    stVector3D points[10000];
    unsigned int numPoints;
    
    struct Triangle triangles[10000];
    unsigned int numTriangles;
    
    stVector3D start;
    stVector3D end;
};

void solveGeometry(stSuperObject *g, struct GeometrySolver *solver);

struct CollisionResult
{
    stVector3D spherePos;
    stVector3D segmentStart;
    stVector3D segmentEnd;
    struct Triangle triangles[1000];
    int n_triangles;
};

#define COLLISION_TYPE_SPHERE   0
#define COLLISION_TYPE_TRIANGLE 1
#define COLLISION_TYPE_L_EDGE   2
#define COLLISION_TYPE_H_EDGE   3

struct Collision
{
    stVector3D normal;
    stVector3D contact;
    stVector3D translation;
    stVector3D movement;
    stVector3D endpos;
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
                          const stMatrix4D dynamicSphereStart,
                          const stMatrix4D dynamicSphereEnd,
                          const stCollideObject *staticObject,
                          const stMatrix4D staticObjectMatrix,
                          struct CollisionRecord *record);

void TransformAndCollide(stDynamics *d, float dt);

#endif /* solver_h */
