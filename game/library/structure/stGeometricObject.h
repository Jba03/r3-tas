#ifndef stGeometricObject_h
#define stGeometricObject_h

#include "structure.h"

typedef struct stGeometricObject tdstGeometricObject;

struct stGeometricObject
{
    readonly pointer points;
    readonly pointer normals;
    readonly doublepointer transparency;
    readonly pointer elementTypes;
    readonly doublepointer elements;
    readonly pointer edges;
    readonly pointer AABBs;
    readonly uint32 type;
    readonly int16 numPoints;
    readonly int16 numElements;
    readonly int16 numEdges;
    readonly int16 numAABB;
    readonly float32 boundingSphereRadius;
    padding(4)
    tdstVector4D boundingSphereCenter;
    readonly pointer edgeList_di;
    readonly int16 numEdges_di;
    readonly int16 numOctreeEdges;
    readonly int32 shadowDrawing;
    readonly int32 shadowConstruction;
    readonly pointer sdc;
    readonly uint32 isStatic;
    readonly uint32 displayList;
    readonly uint8 sinus;
    padding(3)
};

#endif /* stGeometricObject_h */
