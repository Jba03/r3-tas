#ifndef stCollideObject_h
#define stCollideObject_h

#include "stVector4D.hh"
#include "library.hh"
#include "stCollideElementIndexedTriangles.hh"

#define collideObjectIndexedTriangles   1
#define collideObjectFacemap            2
#define collideObjectSprite             3
#define collideObjectTMesh              4
#define collideObjectPoints             5
#define collideObjectLines              6
#define collideObjectIndexedSpheres     7
#define collideObjectAABB               8
#define collideObjectCones              9
#define collideObjectDeformationSetInfo 13

typedef struct stCollideObject stCollideObject;

struct stCollideObject
{
    readonly int16 numVertices;
    readonly int16 numElements;
    readonly int16 numBoundingBoxes;
    readonly padding(2)
    readonly pointer<stVector3D> vertices;
    readonly pointer<int16> elementTypes;
    readonly pointer<> elements;
    readonly pointer<> octree;
    readonly pointer<> boundingBoxes;
    readonly float32 boundingSphereRadius;
    readonly stVector4D boundingSpherePosition;
 
    enum elementType
    {
        INDEXED_TRIANGLES  = 1,
        FACEMAP            = 2,
        SPRITE             = 3,
        TMESH              = 4,
        POINTS             = 5,
        LINES              = 6,
        INDEXED_SPHERES    = 7,
        AABB               = 8,
        CONES              = 9,
        DEFORMATIONSETINFO = 13,
    };
    
  int countOfElements(int selected) {
    int total = 0;
    for (int16_t i = 0; i < numElements; i++) {
      int16 type = elementTypes[i];
      if (type == selected) total++;
    }
    return total;
  }
  
    template <typename F>
    void forEachElement(const F& f)
    {
        for (int16_t i = 0; i < numElements; i++)
        {
            pointer<> dptr = elements[i];
            //void *element = pointer { dptr.address() };
            int16 type = elementTypes[i];
            
            //printf("%X\n", ((uint8_t*)elementTypes - memoryBase));
            f(type, dptr);
        }
    }
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
