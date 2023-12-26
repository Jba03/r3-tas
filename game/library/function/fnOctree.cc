//
//  octree.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-04.
//

#include "stOctree.hh"
#include "memory.hh"

#include "fnPrimIntersections.c"

#define OCTREE_MAX_SELECTED_NODES   100

static void fnOctreeSelectNode(stOctreeNode* node,
                               stOctreeNode** selected,
                               int *n_selected,
                               float *st,
                               float t)
{
    int index = *n_selected;
    while (0 < index && t < st[index - 1])
    {
        if (index < OCTREE_MAX_SELECTED_NODES)
        {
            selected[index] = selected[index - 1];
            st[index] = st[index - 1];
        }
        --index;
    }
    
    if (index < OCTREE_MAX_SELECTED_NODES)
    {
        selected[index] = node;
        st[index] = t;
        if (*n_selected < OCTREE_MAX_SELECTED_NODES) (*n_selected)++;
    }
}

static stOctreeNode* fnOctreeIntersectBox(const stOctreeNode* tree, const stVector3D min, const stVector3D max)
{
    return NULL;
}

/* octree_traverse_line_segment: traverse an octree with a line segment. AB is a vector from point A.  */
static void fnOctreeTraverseLineSegment(const stOctreeNode* node,
                                        const stMatrix4D octree_transform,
                                        const stVector3D A,
                                        const stVector3D AB,
                                        stOctreeNode** selected,
                                        int *n_selected,
                                        float *st)
{
    if (!node) return;
    
    float t;
    
    stVector3D min = vector3_host_byteorder(node->min);
    stVector3D max = vector3_host_byteorder(node->max);
    stVector4D min4 = vector4_mul_matrix4(vector4_new(min.x, min.y, min.z, 1.0f), octree_transform);
    stVector4D max4 = vector4_mul_matrix4(vector4_new(max.x, max.y, max.z, 1.0f), octree_transform);
    min = vector3_new(min4.x, min4.y, min4.z);
    max = vector3_new(max4.x, max4.y, max4.z);
    
    if (fnLineSegmentBoxIntersection(min, max, A, AB, &t))
    {
        const pointer* childlist = (const pointer*)pointer(node->children);
        if (childlist)
        {
            for (int i = 0; i < 8; i++)
            {
                const stOctreeNode* node = (const stOctreeNode*)pointer(*(childlist + i));
                fnOctreeTraverseLineSegment(node, octree_transform, A, AB, selected, n_selected, st);
            }
        }
        else
        {
            const pointer* face_indices = pointer(node->faceIndices);
            if (face_indices) fnOctreeSelectNode((stOctreeNode*)node, selected, n_selected, st, t);
        }
    }
}
