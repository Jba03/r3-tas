#ifndef stOctree_h
#define stOctree_h

#include "library.hh"
#include "stVector3D.hh"
#include "stMatrix4D.hh"

typedef struct stOctree stOctree;
typedef struct stOctreeNode stOctreeNode;

struct stOctree {
    readonly pointer<> root;
    readonly int16 numFaces;
    readonly padding(2)
    readonly pointer<> elementBases;
    readonly stVector3D min;
    readonly stVector3D max;
};

struct stOctreeNode
{
    readonly stVector3D min;
    readonly stVector3D max;
    readonly pointer<uint32_t> children;
    readonly pointer<uint8_t> faceIndices; /* ::uchar8 */
};

#endif /* stOctree_h */
