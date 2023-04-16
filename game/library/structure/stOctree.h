#ifndef stOctree_h
#define stOctree_h

#include "structure.h"
#include "stVector3D.h"
#include "stMatrix4D.h"

typedef struct stOctree tdstOctree;
typedef struct stOctreeNode tdstOctreeNode;

struct stOctree
{
    readonly pointer root;
    readonly int16 numFaces;
    padding(2)
    readonly pointer elementBases;
    readonly tdstVector3D min;
    readonly tdstVector3D max;
};

struct stOctreeNode
{
    readonly tdstVector3D min;
    readonly tdstVector3D max;
    readonly doublepointer children;
    readonly pointer faceIndices; /* ::uchar8 */
};

#endif /* stOctree_h */
