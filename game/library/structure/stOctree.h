#ifndef stOctree_h
#define stOctree_h

#include "structure.h"
#include "stVector3D.h"
#include "stMatrix4D.h"

struct stOctree {
    readonly pointer root;
    readonly int16 n_faces;
    padding(2)
    readonly pointer element_bases;
    readonly tdstVector3D min;
    readonly tdstVector3D max;
} typedef tdstOctree;

struct stOctreeNode {
    readonly tdstVector3D min;
    readonly tdstVector3D max;
    readonly doublepointer children;
    readonly pointer face_indices; /* ::uchar8 */
} typedef tdstOctreeNode;

#endif /* stOctree_h */
