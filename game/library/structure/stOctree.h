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

#if USE_FUNCTIONS

#define OCTREE_MAX_SELECTED_NODES   100

tdstOctreeNode* octree_intersect_box(const tdstOctree* tree, const tdstVector3D min, const tdstVector3D max);

/* octree_traverse_line_segment: traverse an octree with a line segment. AB is a vector from point A.  */
void octree_traverse_line_segment(const tdstOctreeNode* node,
                                  const tdstMatrix4D octree_transform,
                                  const tdstVector3D A,
                                  const tdstVector3D AB,
                                  tdstOctreeNode** selected,
                                  int *n_selected,
                                  float *st);
#endif

#endif /* stOctree_h */
