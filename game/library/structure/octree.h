#ifndef octree_h
#define octree_h

#include "structure.h"
#include "vector3.h"
#include "matrix4.h"

struct octree
{
    readonly pointer root;
    readonly int16 n_faces;
    padding(2)
    readonly pointer element_bases;
    readonly struct vector3 min;
    readonly struct vector3 max;
};

struct octree_node
{
    readonly struct vector3 min;
    readonly struct vector3 max;
    readonly doublepointer children;
    readonly pointer face_indices;
};

#if USE_FUNCTIONS

#define OCTREE_MAX_SELECTED_NODES   100

struct octree_node* octree_intersect_box(const struct octree* tree, const struct vector3 min, const struct vector3 max);

/* octree_traverse_line_segment: traverse an octree with a line segment. AB is a vector from point A.  */
void octree_traverse_line_segment(const struct octree_node* node,
                                  const struct matrix4 octree_transform,
                                  const struct vector3 A,
                                  const struct vector3 AB,
                                  struct octree_node** selected,
                                  int *n_selected,
                                  float *st);
#endif

#endif /* octree_h */
